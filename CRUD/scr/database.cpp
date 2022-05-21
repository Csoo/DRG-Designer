#include "database.h"

enum Language {en = 1, hu = 2};
enum LabelType {title = 1, definition = 2, longdefinition = 3, fullyspecifiedname = 4, inclusion = 5, exclusion = 6, synonym = 7, narrowerterm = 8, simplified = 9};

Database::Database():database(QSqlDatabase::addDatabase("QODBC")) {
}

bool Database::connectDatabase(QString *error) {
    QString connectionString = QStringLiteral(
                            "DRIVER={ODBC Driver 11 for SQL Server};"
                            "SERVER=halado-adatb.mik.uni-pannon.hu,2017;"
                            "UID=icd;"
                            "DATABASE=icd;"
                            "PWD=h6twqPNO;"
                            "Trusted_Connection=No;");
    database.setDatabaseName(connectionString);
    if (!database.open()) {
        if (error != nullptr) {
            *error = database.lastError().text();
        }
            return false;
    }
    return true;
}

void Database::closeDatabase()
{
    database.close();
}

bool Database::testConnection(QString *error)
{
    if (!database.isOpen()) {
        *error = QString("Database is not open.");
        return false;
    }
    query.clear();
    bool isConnected = query.exec("SELECT 1");
    *error = database.lastError().text();

    return isConnected;
}

int Database::login(QString username, QString password)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT id FROM expert_user \
                  WHERE username LIKE :usernameInput AND PWDCOMPARE(:passwordInput, password) = 1");
    query.bindValue(":usernameInput", username);
    query.bindValue(":passwordInput", password);
    query.exec();
    if (query.next()) {
        int userId = query.value(0).toInt();
        return userId;
    } else {
        return -1;
    }
}

void Database::loginLog(int userId)
{
    if (userId != -1) {
        query.clear();
        query.setForwardOnly(true);
        query.prepare("INSERT INTO logging.log_user (user_id, login_date, logout_date) \
                      VALUES (:userId, GETDATE(), NULL)");
        query.bindValue(":userId", userId);
        query.exec();
    }
}

bool Database::logoutLog(int userId)
{
    if (userId != -1) {
        query.clear();
        query.setForwardOnly(true);
        query.prepare("SELECT COUNT (*) \
                        FROM logging.log_user \
                        WHERE user_id = :userId AND logout_date IS NULL");
        query.bindValue(":userId", userId);
        query.exec();
        query.next();
        int sumOfUsers = query.value(0).toInt();
        if (sumOfUsers == 1) {
            query.clear();
            query.setForwardOnly(true);
            query.prepare("UPDATE logging.log_user \
                          SET logout_date = GETDATE() \
                          WHERE user_id = :userId AND logout_date IS NULL");
            query.bindValue(":userId", userId);
            query.exec();
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Database::isFirstLogin(int userId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT COUNT(*) FROM logging.log_user \
                  WHERE user_id = :userId AND logout_date IS NOT NULL");
    query.bindValue(":userId", userId);
    query.exec();
    query.next();
    int sumOfUserId = query.value(0).toInt();

    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT COUNT(*) FROM logging.log_user \
                  WHERE logout_date IS NULL AND user_id = :userId");
    query.bindValue(":userId", userId);
    query.exec();
    query.next();
    int sumOfActiveUserId = query.value(0).toInt();

    if (sumOfUserId == 0 && sumOfActiveUserId == 1) {
        return true;
    } else {
        return false;
    }
}

int Database::createLabel(QString labelText, int labelTypeCode, int langId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT TOP 1 label_id FROM label ORDER BY label_id DESC");
    query.exec();
    query.next();
    int currentId = query.value(0).toInt() + 1;

    database.transaction();
    try {
        query.prepare("INSERT INTO label (label_id, label_type_code) VALUES (:labelId, :labelTypeCode)");
        query.bindValue(":labelId", currentId);
        query.bindValue(":labelTypeCode", labelTypeCode);
        query.exec();
        query.prepare("INSERT INTO label_text (label_id, lang_id, label_text) VALUES (:labelId, :langId, :labelText)");
        query.bindValue(":labelId", currentId);
        query.bindValue(":langId", langId);
        query.bindValue(":labelText", labelText);
        query.exec();
        database.commit();
    } catch(...) {
        currentId = -1;
        database.rollback();
    }

    return currentId;
}

int Database::addNewDrg(QString drgName)
{
    database.transaction();
    query.clear();
    QDateTime now = QDateTime::currentDateTime();
    QString date = now.toString("yyyy-MM-dd");
    // cout << dateNow.toStdString() << endl;
    query.setForwardOnly(true);
    query.prepare("INSERT INTO hbcs.hbcs (hbcs_name_hu, hbcs_created_date, hbcs_last_modified_date) \
                  VALUES (:name, :date, :date)");
    query.bindValue(":name", drgName);
    query.bindValue(":date", date);
    query.exec();
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT TOP 1 hbcs_id FROM hbcs.hbcs \
                   ORDER BY hbcs_id DESC");
    query.exec();
    query.next();
    int newDrgId = query.value(0).toInt();
    database.commit();
    return newDrgId;
}

int Database::addNewDrgChapter(int drgId, QString chapterCode, QString labelText, int langId)
{
    database.transaction();
    int titleId = createLabel(labelText, LabelType::title, langId);
    query.clear();
    query.prepare("INSERT INTO hbcs.hbcs_chapter (hbcs_id, chapter_code, title_id) \
                    VALUES (:drgId, :chapterCode, :titleId)");
    query.bindValue(":drgId", drgId);
    query.bindValue(":chapterCode", chapterCode);
    query.bindValue(":titleId", titleId);
    query.exec();
    query.clear();
    query.prepare("SELECT TOP 1 chapter_id FROM hbcs.hbcs_chapter \
                   ORDER BY chapter_id DESC");
    query.exec();
    query.next();
    int chapterId = query.value(0).toInt();
    database.commit();
    return chapterId;
}

int Database::addNewDrgEntity(int drgId, int chapterId, QString chapterType, QString codeName, QString titleText, QString simplifiedTitleText, int minDayLimit, int maxDayLimit, int normativeDay, float weight, int langId)
{
    database.transaction();
    int titleId = createLabel(titleText, LabelType::title, langId);
    // if not null!
    int simplifiedTitleId = createLabel(simplifiedTitleText, LabelType::simplified, langId);
    query.clear();
    query.setForwardOnly(true);
    query.prepare("INSERT INTO hbcs.hbcs_entity (hbcs_id, chapter_id, chapter_type, code_name, title_id, simplified_title_id, min_day_limit, max_day_limit, normative_day, weight) \
                    VALUES (:drgId, :chapterId, :chapterType, :codeName, :titleId, :simplifiedTitleId, :minDay, :maxDay, :normativeDay, :weight)");
    query.bindValue(":drgId", drgId);
    query.bindValue(":chapterId", chapterId);
    query.bindValue(":chapterType", chapterType);
    query.bindValue(":codeName", codeName);
    query.bindValue(":titleId", titleId);
    query.bindValue(":simplifiedTitleId", simplifiedTitleId);
    query.bindValue(":minDay", minDayLimit);
    query.bindValue(":maxDay", maxDayLimit);
    query.bindValue(":normativeDay", normativeDay);
    query.bindValue(":weight", QString::number(weight));
    query.exec();
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT TOP 1 code_id FROM hbcs.hbcs_entity \
                   ORDER BY code_id DESC");
    query.exec();
    query.next();
    int codeId = query.value(0).toInt();
    database.commit();
    return codeId;
}

int Database::addNewDrgType(int drgId, int codeId, QString typeName)
{
    query.clear();
    // cout << dateNow.toStdString() << endl;
    query.setForwardOnly(true);
    query.prepare("INSERT INTO hbcs.hbcs_type (type_name, hbcs_code_id, hbcs_id) \
                  VALUES (:typeName, :codeId, :drgId)");
    query.bindValue(":typeName", typeName);
    query.bindValue(":codeId", codeId);
    query.bindValue(":drgId", drgId);
    query.exec();
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT TOP 1 type_id FROM hbcs.hbcs_type \
                   ORDER BY type_id DESC");
    query.exec();
    query.next();
    int newDrgType = query.value(0).toInt();
    qDebug() << newDrgType;
    database.commit();
    return newDrgType;
}

int Database::getNewDrgTypeRelationId()
{
    query.exec();
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT TOP 1 relation_id FROM hbcs.hbcs_type_relation \
                   ORDER BY relation_id DESC");
    if (query.next()) {
        return query.value(0).toInt() + 1;
    } else {
        return 1;
    }
}

void Database::addNewDrgTypeRelation(int drgId, int relationId, int relation, int typeId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("INSERT INTO hbcs.hbcs_type_relation (relation_id, hbcs_id, relation, type_id) \
                  VALUES (:relationId, :drgId, :relation, :typeId)");
    query.bindValue(":relationId", relationId);
    query.bindValue(":drgId", drgId);
    query.bindValue(":relation", relation);
    query.bindValue(":typeId", typeId);
    query.exec();
    query.clear();
}

QSqlQuery Database::listDrgs()
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT hbcs_id, hbcs_name_hu, hbcs_created_date, hbcs_last_modified_date \
                    FROM hbcs.hbcs");
    query.exec();
    return query;
}

QSqlQuery Database::listDrgChapters(int drgId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT chapter_id, hbcs_id, chapter_code, label_text \
                    FROM hbcs.hbcs_chapter hbcs_chapter \
                    INNER JOIN label_text ON label_text.label_id=hbcs_chapter.title_id \
                    WHERE hbcs_id = :drgId");
    query.bindValue(":drgId", drgId);
    query.exec();
    return query;
}

QSqlQuery Database::listDrgEntities(int chapterId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT code_id, chapter_id, chapter_type, code_name, titleText.label_text as title_text, simplifiedText.label_text as simplified_text, min_day_limit, max_day_limit, normative_day, weight \
                    FROM hbcs.hbcs_entity hbcsEntity \
                    INNER JOIN label_text titleText ON titleText.label_id = hbcsEntity.title_id \
                    INNER JOIN label_text simplifiedText ON simplifiedText.label_id = hbcsEntity.simplified_title_id \
                    WHERE chapter_id = :chapterId");
    query.bindValue(":chapterId", chapterId);
    query.exec();
    return query;
}

QSqlQuery Database::listDrgTypes(int entityId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT type_id, type_name, hbcs_code_id, hbcs_id \
                    FROM hbcs.hbcs_type \
                    WHERE hbcs_code_id = :entityId");
    query.bindValue(":entityId", entityId);
    query.exec();
    return query;
}

QSqlQuery Database::listDrgTypeRelations(int typeId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT relation_id \
                  FROM hbcs.hbcs_type_relation \
                  WHERE type_id = :typeId");
    query.bindValue(":typeId", typeId);
    query.exec();

    int relationId = -1;
    if (query.next()) {
        relationId = query.value(0).toInt();
    }

    // relation: 1 = ÉS, 2 = VAGY
    query.setForwardOnly(true);
    query.prepare("SELECT typeRelation.hbcs_id, relation_id, typeRelation.type_id, type_name, relation \
                  FROM hbcs.hbcs_type_relation typeRelation \
                  INNER JOIN hbcs.hbcs_type type ON typeRelation.type_id = type.type_id \
                  WHERE relation_id = :relationId");
    query.bindValue(":relationId", relationId);
    query.exec();

    return query;
}

QSqlQuery Database::listIcdDrgRelation(int entityId, int typeId = 0)
{
    QString queryText = "SELECT DISTINCT conceptLinea.concept_id, conceptLinea.concept_type, conceptLinea.concept_code_name, 'label' = "
                        "CASE conceptLinea.concept_type "
                        "WHEN 0 THEN labelText.label_text "
                        "WHEN 1 THEN labelTextLinea.label_text "
                        "WHEN 2 THEN labelTextLinea.label_text END "
                        "FROM hbcs.bno_hbcs_relation bnoHbcsRelation "
                        "INNER JOIN bno10.bno10_entity bno10Entity ON bnoHbcsRelation.bno_entity_id = bno10Entity.entity_id "
                        "LEFT JOIN icd10_bno10_relation icdBnoRelation ON bno10Entity.entity_id = icdBnoRelation.bno10_entity_id "
                        "LEFT JOIN icd10.icd10_entity icd10Entity ON icdBnoRelation.icd10_entity_id = icd10Entity.entity_id "
                        "LEFT JOIN icd11.icd11_icd10_relation icdIcdRelation ON icd10Entity.entity_id = icdIcdRelation.entity_id "
                        "LEFT JOIN icd11.concept_linea conceptLinea ON icdIcdRelation.concept_id = conceptLinea.concept_id "
                        "LEFT JOIN icd11.concept concept ON conceptLinea.concept_id = concept.concept_id "
                        "LEFT JOIN label_text labelText ON concept.title_label_id = labelText.label_id "
                        "LEFT JOIN icd11.concept_linea_label conceptLineaLabel ON conceptLinea.concept_id = conceptLineaLabel.concept_id "
                        "LEFT JOIN label_text labelTextLinea ON conceptLineaLabel.title_label_id = labelTextLinea.label_id "
                        "WHERE conceptLinea.concept_code_name IS NOT NULL ";

    query.setForwardOnly(true);

    if (typeId == NULL) {
        query.prepare(queryText +
                      "AND bnoHbcsRelation.hbcs_code_id = :codeId");
        query.bindValue(":codeId", entityId);
    } else {
        query.prepare(queryText +
                      "AND bnoHbcsRelation.hbcs_code_id = :codeId AND bnoHbcsRelation.hbcs_type_id = :typeId");
        query.bindValue(":codeId", entityId);
        query.bindValue(":typeId", typeId);
    }

    query.exec();

    return query;
}

QSqlQuery Database::listPostCoordinations(int conceptId, int conceptType)
{
    query.clear();
    query.setForwardOnly(true);
    // possible postcoordinations
    query.prepare("SELECT linea.linea_name_en, conceptCoord.axis_id, axis.axis_name_en, \
                  conceptCoord.multip_code, conceptCoord.req, \
                  scaleEntity.scale_entity_id, scaleEntity.scale_entity_concept_type, \
                  conceptLinea.concept_code_name, conceptLinea.concept_code_range, conceptLinea.concept_block, \
                  'label' = CASE scaleEntity.scale_entity_concept_type \
                      WHEN 0 THEN labelTextConcept.label_text \
                      WHEN 1 THEN labelTextLinea.label_text \
                      WHEN 2 THEN labelTextLinea.label_text END, \
                  (SELECT COUNT(*) FROM icd11.concept_linea WHERE parent_concept_id = scaleEntity.scale_entity_id) as 'number_of_children' \
                  FROM icd11.concept_coord conceptCoord \
                  INNER JOIN icd11.scale_entity scaleEntity ON conceptCoord.concept_coord_id = scaleEntity.concept_coord_id \
                  INNER JOIN icd11.axis axis ON conceptCoord.axis_id = axis.axis_id \
                  INNER JOIN icd11.linea linea ON conceptCoord.linea_id = linea.linea_id \
                  INNER JOIN icd11.concept_linea conceptLinea ON scaleEntity.scale_entity_id = conceptLinea.concept_id AND scaleEntity.scale_entity_concept_type = conceptLinea.concept_type \
                  RIGHT JOIN icd11.concept concept ON conceptLinea.concept_id = concept.concept_id AND conceptLinea.concept_type = 0 \
                  LEFT JOIN icd11.concept_linea_label conceptLineaLabel ON conceptLinea.concept_id = conceptLineaLabel.concept_id AND conceptLinea.concept_type = conceptLineaLabel.concept_type \
                  LEFT JOIN label_text labelTextLinea ON conceptLineaLabel.title_label_id = labelTextLinea.label_id \
                  LEFT JOIN label_text labelTextConcept ON concept.title_label_id = labelTextConcept.label_id \
                  WHERE conceptCoord.concept_id = :conceptId AND conceptCoord.concept_type = :conceptType");
    query.bindValue(":conceptId", conceptId);
    query.bindValue(":conceptType", conceptType);
    query.exec();
    return query;
}

QSqlQuery Database::listIcd11Linea(int conceptId, int conceptType)
{
    // without foundation
    query.clear();
    query.setForwardOnly(true);
    // linearization information
    query.prepare("SELECT conceptLinea.concept_id, conceptLinea.concept_type, conceptLinea.concept_code_name, conceptLinea.concept_block, conceptLinea.concept_code_range, \
                  'icd11_label' = \
                  CASE conceptLinea.concept_type \
                      WHEN 0 THEN labelTextConcept.label_text \
                      WHEN 1 THEN labelTextLinea.label_text \
                      WHEN 2 THEN labelTextLinea.label_text END, \
                  conceptLinea.parent_concept_id, 0 as 'parent_concept_type', labelTextParent.label_text as 'parent_label', \
                  linea.linea_name_en, linea.linea_id, \
                  icdIcdRelation.entity_id, icd10Entity.entity_code_name, labelTextIcd10.label_text as 'icd10_label', \
                  icdBnoRelation.bno10_entity_id, bno10Entity.entity_code_name, labelTextBno10.label_text as 'bno10_label', \
                  'icd11_def_label' = \
                  CASE conceptLinea.concept_type \
                      WHEN 0 THEN labelTextDef.label_text \
                      ELSE NULL END \
                  FROM icd11.concept_linea conceptLinea \
                  INNER JOIN icd11.linea linea ON conceptLinea.linea_id = linea.linea_id \
                  FULL JOIN icd11.icd11_icd10_relation icdIcdRelation ON conceptLinea.concept_id = icdIcdRelation.concept_id AND conceptLinea.concept_type = icdIcdRelation.concept_type \
                  FULL JOIN icd10.icd10_entity icd10Entity ON icdIcdRelation.entity_id = icd10Entity.entity_id \
                  LEFT JOIN icd10_bno10_relation icdBnoRelation ON icd10Entity.entity_id = icdBnoRelation.icd10_entity_id \
                  LEFT JOIN bno10.bno10_entity bno10Entity ON icdBnoRelation.bno10_entity_id = bno10Entity.entity_id \
                  INNER JOIN icd11.concept concept ON conceptLinea.concept_id = concept.concept_id AND conceptLinea.concept_type = 0 \
                  LEFT JOIN label_text labelTextConcept ON concept.title_label_id = labelTextConcept.label_id \
                  LEFT JOIN icd11.concept_linea_label conceptLineaLabel ON conceptLinea.concept_id = conceptLineaLabel.concept_id AND conceptLinea.concept_type = conceptLineaLabel.concept_type \
                  LEFT JOIN label_text labelTextLinea ON conceptLineaLabel.title_label_id = labelTextLinea.label_id \
                  INNER JOIN icd11.concept parentConcept ON conceptLinea.parent_concept_id = parentConcept.concept_id \
                  INNER JOIN label_text labelTextParent ON parentConcept.title_label_id = labelTextParent.label_id \
                  FULL JOIN label_text labelTextIcd10 ON icd10Entity.title_label_id = labelTextIcd10.label_id \
                  FULL JOIN label_text labelTextBno10 ON bno10Entity.title_label_id = labelTextBno10.label_id \
                  FULL JOIN label_text labelTextDef ON concept.def_label_id = labelTextDef.label_id \
                  WHERE conceptLinea.concept_id = :conceptId AND conceptLinea.concept_type = :conceptType");
    query.bindValue(":conceptId", conceptId);
    query.bindValue(":conceptType", conceptType);
    query.exec();
    return query;
}

QSqlQuery Database::searchIcd11(QString searchText, bool isDefault)
{
    QString queryText = "SELECT searchResult.concept_id, searchResult.concept_type, searchResult.concept_code_name, searchResult.label FROM "
            "(SELECT conceptLinea.concept_code_name, "
            "conceptLinea.concept_id, conceptLinea.concept_type, label =  "
            "CASE conceptLinea.concept_type "
            "WHEN 0 THEN labelText.label_text "
            "WHEN 1 THEN labelTextLinea.label_text "
            "WHEN 2 THEN labelTextLinea.label_text END "
            "FROM icd11.concept_linea conceptLinea "
            "LEFT JOIN icd11.concept_linea_label conceptLineaLabel ON conceptLinea.concept_id = conceptLineaLabel.concept_id AND conceptLinea.concept_type = conceptLineaLabel.concept_type "
            "LEFT JOIN icd11.concept concept ON conceptLinea.concept_id = concept.concept_id "
            "LEFT JOIN label_text labelText ON concept.title_label_id = labelText.label_id "
            "LEFT JOIN label_text labelTextLinea ON conceptLineaLabel.title_label_id = labelTextLinea.label_id) as searchResult "
            "WHERE (searchResult.label LIKE :searchText OR searchResult.concept_id LIKE :searchText OR searchResult.concept_code_name LIKE :searchText) "
            "AND searchResult.concept_code_name IS NOT NULL";
    query.clear();
    query.setForwardOnly(true);
    query.prepare(queryText);
    query.bindValue(":searchText", QString("%%1%").arg(searchText));
    query.exec();

    return query;
}

QSqlQuery Database::listOrderedDrgBno(int chapterId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT hbcsEntity.code_id, hbcsEntity.code_name, hbcsCodeLabel.label_text, \
                  hbcsType.type_id, hbcsType.type_name, \
                  bno10Entity.entity_id, bno10Entity.entity_code_name, bno10Label.label_text, \
                  bnoHbcsRelation.is_approved \
                  FROM hbcs.hbcs_entity hbcsEntity \
                  LEFT JOIN hbcs.hbcs_type hbcsType ON hbcsEntity.code_id = hbcsType.hbcs_code_id \
                  LEFT JOIN hbcs.bno_hbcs_relation bnoHbcsRelation ON hbcsType.type_id = bnoHbcsRelation.hbcs_type_id \
                  LEFT JOIN bno10.bno10_entity bno10Entity ON bnoHbcsRelation.bno_entity_id = bno10Entity.entity_id \
                  LEFT JOIN label_text hbcsCodeLabel ON hbcsEntity.title_id = hbcsCodeLabel.label_id \
                  LEFT JOIN label_text bno10Label ON bno10Entity.title_label_id = bno10Label.label_id \
                  WHERE hbcsEntity.chapter_id = :chapterId \
                  ORDER BY hbcsEntity.code_name, hbcsType.type_id, bno10Entity.entity_code_name");
    query.bindValue(":chapterId", chapterId);
    query.exec();
    return query;
}

void Database::approveBnoDrgRelation(int entityId, int drgCodeId, int drgTypeId, int drgId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("UPDATE hbcs.bno_hbcs_relation \
                  SET is_approved = 1 \
                  WHERE hbcs_code_id = :drgCodeId AND bno_entity_id = :bnoEntityId AND hbcs_type_id = :drgTypeId AND hbcs_id = :drgId");
    query.bindValue(":bnoEntityId", entityId);
    query.bindValue(":drgCodeId", drgCodeId);
    query.bindValue(":drgTypeId", drgTypeId);
    query.bindValue(":drgId", drgId);
    query.exec();
}

void Database::insertIcd11BnoRelationTable(int icd11DrgRelationId, int entityId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("INSERT INTO bno10.icd11_bno_relation (icd11_hbcs_relation_id, bno10_entity_id) \
                  VALUES (:relationId, :entityId)");
    query.bindValue(":relationId", icd11DrgRelationId);
    query.bindValue(":entityId", entityId);
    query.exec();
}

int Database::getNewIcd11DrgRelationId()
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT TOP 1 * FROM hbcs.icd11_hbcs_relation \
                  ORDER BY relation_id ASC");
    query.exec();
    int relationId;
    if (query.next()) {
        relationId = query.value(0).toInt() + 1;
    } else {
        relationId = 1;
    }
    return relationId;
}

void Database::addStemToIcd11DrgRelation(int relationId, int drgId, int drgCodeId, int drgTypeId, int conceptId, int conceptType, int axisId = NULL)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("INSERT INTO hbcs.icd11_hbcs_relation (relation_id, hbcs_id, hbcs_code_id, hbcs_type_id, icd11_concept_id, icd11_concept_type, axis_id) \
                   VALUES (:relationId, :drgId, :drgCodeId, :drgTypeId, :conceptId, :conceptType, :axisId)");
    query.bindValue(":relationId", relationId);
    query.bindValue(":drgId", drgId);
    query.bindValue(":drgCodeId", drgCodeId);
    query.bindValue(":drgTypeId", drgTypeId);
    query.bindValue(":conceptId", conceptId);
    query.bindValue(":conceptType", conceptType);
    query.bindValue(":axisId", axisId);
    query.exec();
}

void Database::addExtensionToIcd11DrgRelation(int relationId, int stemConceptId, int stemConceptType, int extensionConceptId, int extensionConceptType, int extensionAxisId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("INSERT INTO hbcs.icd11_extension_relation (icd11_hbcs_relation_id, stem_concept_id, stem_concept_type, extension_concept_id, extension_concept_type, extension_axis_id) \
                   VALUES (:relationId, :stemConceptId, :stemConceptType, :conceptId, :conceptType, :axisId)");
    query.bindValue(":relationId", relationId);
    query.bindValue(":stemConceptId", stemConceptId);
    query.bindValue(":stemConceptType", stemConceptType);
    query.bindValue(":extensionConceptId", extensionConceptId);
    query.bindValue(":extensionConceptType", extensionConceptType);
    query.bindValue(":extensionAxisId", extensionAxisId);
    query.exec();
}

QSqlQuery Database::getPostcoordinationChild(int parentConceptId, int conceptType)
{
    query.clear();
    query.setForwardOnly(true);
    if (conceptType == 0) {
        query.prepare("SELECT conceptLinea.concept_id, conceptLinea.concept_type, conceptLinea.concept_code_name, conceptLinea.concept_block, conceptLinea.concept_code_range, \
                      'icd11_label' =  \
                      CASE conceptLinea.concept_type \
                          WHEN 0 THEN labelTextConcept.label_text \
                          WHEN 1 THEN labelTextLinea.label_text \
                          WHEN 2 THEN labelTextLinea.label_text END, \
                      linea.linea_name_en, linea.linea_id, \
                      (SELECT COUNT(*) FROM icd11.concept_linea WHERE parent_concept_id = conceptLinea.concept_id AND conceptLinea.concept_type = 0) as 'number_of_children' \
                      FROM icd11.concept_linea conceptLinea \
                      INNER JOIN icd11.linea linea ON conceptLinea.linea_id = linea.linea_id \
                      LEFT JOIN icd11.concept concept ON conceptLinea.concept_id = concept.concept_id AND conceptLinea.concept_type = 0 \
                      LEFT JOIN label_text labelTextConcept ON concept.title_label_id = labelTextConcept.label_id \
                      LEFT JOIN icd11.concept_linea_label conceptLineaLabel ON conceptLinea.concept_id = conceptLineaLabel.concept_id AND conceptLinea.concept_type = conceptLineaLabel.concept_type \
                      LEFT JOIN label_text labelTextLinea ON conceptLineaLabel.title_label_id = labelTextLinea.label_id \
                      WHERE conceptLinea.parent_concept_id = :conceptId");
        query.bindValue(":conceptId", parentConceptId);
        query.exec();
    }
    return query;
}

QSqlQuery Database::listMappingSuggestionToBno10(int entityId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("SELECT icdIcdRelation.relation_id, icdIcdRelation.concept_id, icdIcdRelation.concept_type, \
                  conceptLinea.concept_code_name, conceptLinea.concept_code_range, conceptLinea.concept_block, \
                  'icd11_label' = CASE conceptLinea.concept_type \
                    WHEN 0 THEN labelTextConcept.label_text \
                    WHEN 1 THEN labelTextLinea.label_text \
                    WHEN 2 THEN labelTextLinea.label_text END \
                  FROM icd10_bno10_relation icdBnoRelation \
                  INNER JOIN icd10.icd10_entity icd10Entity ON icdBnoRelation.icd10_entity_id = icd10Entity.entity_id \
                  INNER JOIN icd11.icd11_icd10_relation icdIcdRelation ON icd10Entity.entity_id = icdIcdRelation.entity_id \
                  INNER JOIN icd11.concept_linea conceptLinea ON icdIcdRelation.concept_id = conceptLinea.concept_id AND icdIcdRelation.concept_type = conceptLinea.concept_type \
                  LEFT JOIN icd11.concept concept ON conceptLinea.concept_id = concept.concept_id AND conceptLinea.concept_type = 0 \
                  LEFT JOIN icd11.concept_linea_label conceptLineaLabel ON conceptLinea.concept_id = conceptLineaLabel.concept_id AND conceptLinea.concept_type = conceptLineaLabel.concept_type \
                  LEFT JOIN label_text labelTextConcept ON concept.title_label_id = labelTextConcept.label_id \
                  LEFT JOIN label_text labelTextLinea ON conceptLineaLabel.title_label_id = labelTextLinea.label_id \
                  WHERE icdBnoRelation.bno10_entity_id = :entityId \
                  ORDER BY icdIcdRelation.relation_id ASC");
    query.bindValue(":entityId", entityId);
    query.exec();
    return query;
}

void Database::deleteDrgIcd11ClusterRelation(int icd11DrgRelationId, int drgId, int drgCodeId, int drgTypeId)
{
    database.transaction();
    try {
        query.clear();
        query.setForwardOnly(true);
        query.prepare("DELETE FROM hbcs.icd11_extension_relation \
                      WHERE icd11_hbcs_relation_id = :relationId");
        query.bindValue(":relationId", icd11DrgRelationId);
        query.exec();

        query.clear();
        query.setForwardOnly(true);
        query.prepare("DELETE FROM hbcs.icd11_hbcs_relation \
                      WHERE relation_id = :relationId");
        query.bindValue(":relationId", icd11DrgRelationId);
        query.exec();

        query.clear();
        query.setForwardOnly(true);
        query.prepare("SELECT bno_entity_id FROM bno10.icd11_bno_relation \
                      WHERE relation_id = :relationId");
        query.bindValue(":relationId", icd11DrgRelationId);
        query.exec();

        QList<QSqlQuery> bnoEntities;
        bnoEntities.append(query);

        for (const auto& bnoEntity : bnoEntities) {
            int bnoId = bnoEntity.value(0).toInt();
            query.clear();
            query.setForwardOnly(true);
            query.prepare("UPDATE hbcs.bno_hbcs_relation \
                          SET is_approved = 0 \
                          WHERE hbcs_code_id = :drgCodeId AND bno_entity_id = :bnoEntityId \
                          AND hbcs_type_id = :drgTypeId AND hbcs_id = :drgId");
            query.bindValue(":drgCodeId", drgCodeId);
            query.bindValue(":bnoEntityId", bnoId);
            query.bindValue(":drgTypeId", drgTypeId);
            query.bindValue(":drgId", drgId);
            query.exec();
        }

        query.clear();
        query.setForwardOnly(true);
        query.prepare("DELETE FROM bno10.icd11_bno_relation \
                      WHERE icd11_hbcs_relation_id = :relationId");
        query.bindValue(":relationId", icd11DrgRelationId);
        query.exec();
        database.commit();
    } catch(...) {
        database.rollback();
    }
}

void Database::updatedDrgSystem(QString drgId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("UPDATE hbcs.hbcs \
                  SET hbcs_last_modified_date = GETDATE() \
                  WHERE hbcs_id = :drgId");
    query.bindValue(":drgId", drgId);
    query.exec();
}

void Database::updateDrg(int drgId, QString drgName, int labelId, QString labelText = NULL, int langId = NULL)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("UPDATE hbcs.hbcs \
                  SET hbcs_name_hu = :drgName, hbcs_last_modified_date = GETDATE() \
                  WHERE hbcs_id = :drgId");
    query.bindValue(":drgId", drgId);
    query.bindValue(":drgName", drgName);
    query.exec();

    if (labelText != NULL || langId != NULL) {
        updateLabel(labelId, labelText, langId);
    }
}

void Database::updateDrgChapter(int drgId, int chapterId, QString chapterCode, int labelId, QString labelText = NULL, int langId = NULL)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("UPDATE hbcs.hbcs_chapter \
                  SET chapter_code = :chapterCode \
                  WHERE chapter_id = :chapterId AND drgId = :drgId");
    query.bindValue(":chapterCode", chapterCode);
    query.bindValue(":chapterId", chapterId);
    query.bindValue(":drgId", drgId);
    query.exec();

    if (labelText != NULL || langId != NULL) {
        updateLabel(labelId, labelText, langId);
    }
}

void Database::updateDrgEntity(int drgId, int chapterId, int codeId, QString chapterType, QString codeName, int titleLabelId, QString titleText, int titleLangId, int simplifiedLabelId, QString simplifiedText, int simplifiedLangId, int minDayLimit, int maxDayLimit, int normativeDay, int weight)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("UPDATE hbcs.hbcs_entity \
                  SET chapter_type = :chapterType, code_name = :codeName, min_day_limit = :minDayLimit, max_day_limit = :maxDayLimit, normative_day = :normativeDay, weight = :weight \
                  WHERE hbcs_id = :drgId AND chapter_id = chapterId AND code_id = :codeId");
    query.bindValue(":drgId", drgId);
    query.bindValue(":chapterId", chapterId);
    query.bindValue(":codeId", codeId);
    query.bindValue(":chapterType", chapterType);
    query.bindValue(":codeName", codeName);
    query.bindValue(":minDayLimit", minDayLimit);
    query.bindValue(":maxDayLimit", maxDayLimit);
    query.bindValue(":normativeDay", normativeDay);
    query.bindValue(":weight", weight);
    query.exec();

    updateLabel(titleLabelId, titleText, titleLangId);
    updateLabel(simplifiedLabelId, simplifiedText, simplifiedLangId);
}

void Database::updateDrgType(int drgId, int typeId, int drgCodeId, QString typeName)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("UPDATE hbcs.hbcs_type \
                  SET type_name = :typeName \
                  WHERE hbcs_id = :drgId AND hbcs_code_id = :drgCodeId AND type_id = :typeId");
    query.bindValue(":drgId", drgId);
    query.bindValue(":drgCodeId", drgCodeId);
    query.bindValue(":typeId", typeId);
    query.bindValue(":typeName", typeName);
    query.exec();
}

void Database::updateDrgTypeRelation(int drgId, int typeId, int relation)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("UPDATE hbcs.hbcs_type_relation \
                  SET relation = :relation \
                  WHERE hbcs_id = :drgId AND type_id = :typeId");
    query.bindValue(":drgId", drgId);
    query.bindValue(":typeId", typeId);
    query.bindValue(":relation", relation);
    query.exec();
}

void Database::updateLabel(int labelId, QString labelText, int langId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("UPDATE dbo.label_text \
                  SET label_text = :labelText, lang_id = :langId \
                  WHERE label_id = :labelId");
    query.bindValue(":labelText", labelText);
    query.bindValue(":langId", langId);
    query.bindValue(":labelId", labelId);
    query.exec();
}

void Database::deleteDrg(int drgId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("DELETE FROM hbcs.hbcs \
                  WHERE hbcs_id = :drgId");
    query.bindValue(":drgId", drgId);
    query.exec();
}

void Database::deleteDrgChapter(int drgId, int chapterId, int labelId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("DELETE FROM hbcs.hbcs_chapter \
                  WHERE hbcs_id = :drgId AND chapter_id = :chapterId");
    query.bindValue(":drgId", drgId);
    query.bindValue(":chapterId", chapterId);
    query.exec();

    deleteLabel(labelId);
}

void Database::deleteDrgEntity(int drgId, int chapterId, int codeId, int titleId, int simplifiedId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("DELETE FROM hbcs.hbcs_entity \
                  WHERE hbcs_id = :drgId AND chapter_id = :chapterId AND code_id = :codeId");
    query.bindValue(":drgId", drgId);
    query.bindValue(":chapterId", chapterId);
    query.bindValue(":codeId", codeId);
    query.exec();

    deleteLabel(titleId);
    deleteLabel(simplifiedId);
}

void Database::deleteDrgType(int drgId, int codeId, int typeId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("DELETE FROM hbcs.hbcs_type \
                  WHERE hbcs_id = :drgId AND hbcs_code_id = :codeId AND type_id = :typeId");
    query.bindValue(":drgId", drgId);
    query.bindValue(":typeId", typeId);
    query.bindValue(":codeId", codeId);
    query.exec();
}

void Database::deleteDrgTypeRelation(int drgId, int typeId, int relationId)
{
    query.clear();
    query.setForwardOnly(true);
    database.transaction();
    try {
        query.prepare("DELETE FROM hbcs.hbcs_type_relation \
                      WHERE hbcs_id = :drgId AND type_id = :typeId AND relation_id = :relationId");
        query.bindValue(":drgId", drgId);
        query.bindValue(":typeId", typeId);
        query.bindValue(":relationId", relationId);
        query.exec();

        query.clear();
        query.setForwardOnly(true);
        query.prepare("SELECT COUNT(*) FROM hbcs.hbcs_type_relation \
                      WHERE hbcs_id = :drgId AND relation_id = :relationId");
        query.bindValue(":drgId", drgId);
        query.bindValue(":relationId", relationId);
        query.exec();
        if (query.next()) {
            int countRelationId = query.value(0).toInt();
            if (countRelationId == 1) {
                query.clear();
                query.setForwardOnly(true);
                query.prepare("DELETE FROM hbcs.hbcs_type_relation \
                              WHERE hbcs_id = :drgId AND relation_id = :relationId");
                query.bindValue(":drgId", drgId);
                query.bindValue(":relationId", relationId);
                query.exec();
            }
        }
        database.commit();
    } catch(...) {
        database.rollback();
    }
}

void Database::deleteLabel(int labelId)
{
    database.transaction();
    try {
        query.clear();
        query.setForwardOnly(true);
        query.prepare("DELETE FROM dbo.label_text \
                      WHERE label_id = :labelId");
        query.bindValue(":labelId", labelId);
        query.exec();

        query.clear();
        query.setForwardOnly(true);
        query.prepare("DELETE FROM dbo.label \
                      WHERE label_id = :labelId");
        query.bindValue(":labelId", labelId);
        query.exec();
        database.commit();
    } catch(...) {
        database.rollback();
    }
}

void Database::deleteBnoHbcsRelationTable(int drgId, int drgCodeId, int drgTypeId, int bnoEntityId)
{
    query.clear();
    query.setForwardOnly(true);
    query.prepare("DELETE FROM hbcs.bno_hbcs_relation \
                  WHERE hbcs_code_id = :drgCodeId AND bno_entity_id = :bnoEntityId \
                  AND hbcs_type_id = :drgTypeId AND hbcs_id = :drgId");
    query.bindValue(":drgCodeId", drgCodeId);
    query.bindValue(":bnoEntityId", bnoEntityId);
    query.bindValue(":drgTypeId", drgTypeId);
    query.bindValue(":drgId", drgId);
    query.exec();
}
