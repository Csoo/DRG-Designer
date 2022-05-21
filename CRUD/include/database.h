#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

using namespace std;

class Database
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");
    QSqlQuery query;
public:
    Database();
    bool connectDatabase(QString *error);
    void closeDatabase();

    bool testConnection(QString *error);

    int login(QString username, QString password);
    void loginLog(int userId);
    bool logoutLog(int userId);
    bool isFirstLogin(int userId);

    int createLabel(QString labelText, int labelTypeCode, int langId);

    int addNewDrg(QString drgName);
    int addNewDrgChapter(int drgId, QString chapterCode, QString labelText, int langId);
    int addNewDrgEntity(int drgId, int chapterId, QString chapterType, QString codeName, QString titleText, QString simplifiedTitleText, int minDayLimit, int maxDayLimit, int normativeDay, float weight, int langId);
    int addNewDrgType(int drgId, int codeId, QString typeName);
    int getNewDrgTypeRelationId();
    void addNewDrgTypeRelation(int drgId, int relationId, int relation, int typeId); //getNewDrgTypeRelationId is needed for relationId parameter

    QSqlQuery listDrgs();
    QSqlQuery listDrgChapters(int drgId);
    QSqlQuery listDrgEntities(int chapterId);
    QSqlQuery listDrgTypes(int entityId);
    QSqlQuery listDrgTypeRelations(int typeId);
    QSqlQuery listIcdDrgRelation(int entityId, int typeId);
    QSqlQuery listIcd11Linea(int conceptId, int conceptType);
    QSqlQuery listPostCoordinations(int conceptId, int conceptType);

    QSqlQuery searchIcd11(QString searchText, bool isDefault);
    QSqlQuery listOrderedDrgBno(int chapterId);
    QSqlQuery listIcd11InDrg();

    int getNewIcd11DrgRelationId();
    void addStemToIcd11DrgRelation(int relationId, int drgId, int drgCodeId, int drgTypeId, int conceptId, int conceptType, int axisId); //declare relId with getNewIcd11DrgRelId before calling this
    void addExtensionToIcd11DrgRelation(int relationId, int stemConceptId, int stemConceptType, int extensionConceptId, int extensionConceptType, int extensionAxisId); //and this
    //these two functions used only in case of switching bno10 to icd11:
    void insertIcd11BnoRelationTable(int icd11DrgRelationId, int entityId); //call only once after declare relId
    void approveBnoDrgRelation(int entityId, int drgCodeId, int drgTypeId, int drgId);

    QSqlQuery getPostcoordinationChild(int conceptId, int conceptType);

    QSqlQuery listMappingSuggestionToBno10(int entityId); //according to the WHO mapping tables
    void deleteDrgIcd11ClusterRelation(int icd11DrgRelationId, int drgId, int drgCodeId, int drgTypeId); // itt lehet, hogy kéne egy változó arra, hogy a bno10-icd11 kapcsolatot is törölje-e avagy sem
    // frontend: postcoord and icd11_drg_relation edit: first: deleteExisting, then addStem and addExtension functions

    void updatedDrgSystem(QString drgId);
    void updateDrg(int drgId, QString drgName, int labelId, QString labelText, int langId);
    void updateDrgChapter(int drgId, int chapterId, QString chapterCode, int labelId, QString labelText, int langId);
    void updateDrgEntity(int drgId, int chapterId, int codeId, QString chapterType, QString codeName, int titleLabelId, QString titleText, int titleLangId, int simplifiedLabelId, QString simplifiedText, int simplifiedLangId, int minDayLimit, int maxDayLimit, int normativeDay, int weight);
    void updateDrgType(int drgId, int typeId, int drgCodeId, QString typeName);
    void updateDrgTypeRelation(int drgId, int typeId, int relation);
    void updateLabel(int labelId, QString labelText, int langId);

    void deleteDrg(int drgId);
    void deleteDrgChapter(int drgId, int chapterId, int labelId);
    void deleteDrgEntity(int drgId, int chapterId, int codeId, int titleId, int simplifiedId);
    void deleteDrgType(int drgId, int codeId, int typeId);
    void deleteDrgTypeRelation(int drgId, int typeId, int relationId);
    void deleteLabel(int labelId);

    void deleteBnoHbcsRelationTable(int drgId, int drgCodeId, int drgTypeId, int bnoEntityId);
};

#endif // DATABASE_H
