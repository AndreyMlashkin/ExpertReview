#ifndef TREERIGHTSIDEVALUESJSON_H
#define TREERIGHTSIDEVALUESJSON_H

#include <QJsonObject>
#include "nodesinfo/treerightsidevalues.h"

class PropertyNodeJson;

class TreeRightSideValuesJson : public TreeRightSideValues
{
public:
    TreeRightSideValuesJson(const QJsonObject &_leftSide);

    QMap<QString, double> values() const override; // key - value
    void setValues(const QMap<QString, double>& _values) override;

    void readValues(const QString& _treeName) override;
    void writeValues(const QString& _treeName) override;

private:
    QJsonObject addValues(QJsonObject _obj);
    QMap<QString, double> extractValues(const QJsonObject& _jObject) const;

    static QString extension();

private:
    QJsonObject m_json;
    QString m_openedFile;

    QMap<QString, double> m_values;
};

#endif // TREERIGHTSIDEVALUESJSON_H
