#ifndef TREERIGHTSIDEVALUESJSON_H
#define TREERIGHTSIDEVALUESJSON_H

#include <QJsonObject>
#include "serialization/nodesinfo/treerightsidevalues.h"

class PropertyNodeJson;

class TreeRightSideValuesJson : public TreeRightSideValues
{
public:
    TreeRightSideValuesJson(const QJsonObject &_leftSide);
    ~TreeRightSideValuesJson();

    QMap<QString, double> values() const override; // key - value
    void setValues(const QMap<QString, double>& _values) override;

    void readValues(const QString& _id) override;
    void writeValues(const QString& _id) override;

    QString guiName() const;
    void setGuiName(const QString &_guiName);

private:
    QJsonObject addValues(QJsonObject _obj);
    QMap<QString, double> extractValues(const QJsonObject& _jObject) const;

private:
    QJsonObject m_json;
    QString m_openedFile;

    QMap<QString, double> m_values;
    QString m_guiName;
};

#endif // TREERIGHTSIDEVALUESJSON_H
