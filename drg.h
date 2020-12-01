#ifndef DRG_H
#define DRG_H

#include "treeitem.h"
#include "QModelIndex"
#include "enum.hpp"

class DRG : public TreeItem
{
    Q_OBJECT

    Q_PROPERTY(unsigned int minDay READ getMinDay NOTIFY attributeChanged)
    Q_PROPERTY(unsigned int maxDay READ getMaxDay NOTIFY attributeChanged)
    Q_PROPERTY(float normativeDay READ getNormativeDay NOTIFY attributeChanged)
    Q_PROPERTY(float weight READ getWeight NOTIFY attributeChanged)
    Q_PROPERTY(QString chapter READ getChapter NOTIFY attributeChanged)
    Q_PROPERTY(QString simpleTitle READ getSimpleTitle NOTIFY attributeChanged)
private:
    unsigned int minDay;
    unsigned int maxDay;
    float normativeDay;
    float weight;
    QString chapter;
    QString simpleTitle;
public:
    DRG();
    DRG(unsigned int id, const QString &code = "", const QString &title = "", TreeItem *parentItem = nullptr);
    void setAttributes(unsigned int minDay, unsigned int maxDay, float normativeDay, float weight, QString simpleTitle);

    unsigned int getMinDay() const;
    unsigned int getMaxDay() const;
    float getNormativeDay() const;
    float getWeight() const;
    QString getSimpleTitle() const;
    QString getChapter() const;

    Q_INVOKABLE void setAttributesFromModel(const QModelIndex &index);

signals:
    void attributeChanged();
};

#endif // DRG_H
