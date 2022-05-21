#include "icd11.h"
#include "treemodel.h"

int ICD11::getConceptType() const
{
    return conceptType;
}

bool ICD11::getIsApproved() const
{
    return isApproved;
}

void ICD11::setIsApproved(bool value)
{
    isApproved = value;
    isApprovedChanged();
}

void ICD11::setConceptType(int value)
{
    conceptType = value;
    attributesChanged();
}

void ICD11::addPostCoordination(ICD11 *icd)
{
    this->postCoordinations.append(icd);
}

void ICD11::recommendationResult(bool noRecommendation, ICD11 *rec)
{
    if (noRecommendation)
        return;
    this->id = rec->getId();
    this->conceptType = rec->getConceptType();
    this->title = rec->getTitle();
    this->code = rec->getCode();
    this->postCoordinations = rec->getPostCoordinations();
    emit attributesChanged();
    emit whoSuggestionReady(noRecommendation);
    delete rec;
}

void ICD11::detailsResult(ICD11 *icd)
{
    if (this->getCode().endsWith("Z"))
        this->setDescription(tr("'Nem meghatározott' fennmaradó kategória."));
    else if (this->getCode().endsWith("Y"))
        this->setDescription(tr("'Egyéb' fennmaradó kategória."));
    else
        this->setDescription(icd->getDescription());
    delete icd;
}

QList<ICD11 *> ICD11::getPostCoordinations() const
{
    return postCoordinations;
}

void ICD11::setPostCoordinations(const QList<ICD11 *> &value)
{
    postCoordinations = value;
}

QString ICD11::getIcd10Code() const
{
    return icd10Code;
}

void ICD11::setIcd10Code(const QString &value)
{
    icd10Code = value;
}

QString ICD11::getIcd10Title() const
{
    return icd10Title;
}

void ICD11::setIcd10Title(const QString &value)
{
    icd10Title = value;
}

int ICD11::getIcd10Id() const
{
    return icd10Id;
}

void ICD11::setIcd10Id(int value)
{
    icd10Id = value;
}

ICD11::ICD11()
{

}

ICD11::ICD11(Repository *db, QThread *dbThread):
    db(db),
    dbThread(dbThread)
{
    qRegisterMetaType<ICD11*>("ICD11*");
    connect(this, &ICD11::startLoadRecommendation, db, &Repository::loadRecommendation);
    connect(db, &Repository::recommendationReady, this, &ICD11::recommendationResult);

    connect(this, &ICD11::startLoadICDDetails, db, &Repository::loadICDDetails);
    connect(db, &Repository::ICDDetailsReady, this, &ICD11::detailsResult);
}

ICD11::ICD11(unsigned int id, int conceptType, const QString &code, const QString &title, bool approved, TreeItem *parentItem):
    TreeItem(Type::ICD11, id, code, title, parentItem),
    isApproved(approved),
    conceptType(conceptType)
{

}

ICD11::~ICD11()
{
    qDeleteAll(postCoordinations);
}

void ICD11::fetchFromModel(const QModelIndex &index)
{
    ICD11 *icd = dynamic_cast<ICD11*>(this->model->getItem(index));
    if (!icd->getIsApproved()) {
        this->icd10Id = icd->getId();
        this->icd10Title = icd->getTitle();
        this->icd10Code = icd->getCode();
    } else {
        TreeItem::fetchFromModel(index);
        conceptType = icd->getConceptType();
    }
    this->postCoordinations = icd->getPostCoordinations();
    this->isApproved = icd->getIsApproved();

    emit attributesChanged();
    emit icdChanged();
}

QString ICD11::getParentCode() const
{
    //
}

void ICD11::loadDetails()
{
    emit startLoadICDDetails(this->id, this->conceptType);
}

void ICD11::loadRecommandation()
{
    emit startLoadRecommendation(this->icd10Id);
}

int ICD11::getPostCoordCount() const
{
    return postCoordinations.size();
}

bool ICD11::hasPostCoord() const
{
    return postCoordinations.size() > 0;
}

QString ICD11::getFullCode() const
{
    QString fullCode = this->code;
    for (auto postCoord : this->postCoordinations) {
        if (postCoord->getCode().mid(0, 1) == 'X') {
            fullCode += '&';
        } else {
            fullCode += '/';
        }
        fullCode += postCoord->getCode();
    }
    return fullCode;
}

QString ICD11::getPostCode(int idx) const
{
    return this->postCoordinations[idx]->getCode();
}

QString ICD11::getPostTitle(int idx) const
{
    return this->postCoordinations[idx]->getTitle();
}

void ICD11::approveICD(const QModelIndex &item)
{

    ICD11 *icdCurrnet = dynamic_cast<ICD11*>(model->getItem(item));
    if (icdCurrnet) {
        icdCurrnet->setIcd10Id(icdCurrnet->getId());
        icdCurrnet->setIcd10Code(icdCurrnet->getCode());
        icdCurrnet->setIcd10Title(icdCurrnet->getTitle());
        icdCurrnet->setId(this->getId());
        icdCurrnet->setTitle(this->getTitle());
        icdCurrnet->setConceptType(this->getConceptType());
        icdCurrnet->setCode(this->getCode());
        icdCurrnet->setPostCoordinations(this->getPostCoordinations());
        icdCurrnet->setIsApproved(true);
    }
    model->dataChanged(item, item);
}

void ICD11::updatePostCoord(unsigned int id, const QString &code, const QString &title)
{
    int idx = 0;
    while (idx < postCoordinations.size() && postCoordinations[idx]->getId() != id)  {
        idx++;
    }
    if (idx >= postCoordinations.size()) {
        postCoordinations.append(new ICD11(id, 0, code, title));
    } else {
        postCoordinations.removeAt(idx);
    }

    emit postCoordCountChanged();
}

void ICD11::clearPostCoord()
{
    postCoordinations.clear();
    emit postCoordCountChanged();
}
