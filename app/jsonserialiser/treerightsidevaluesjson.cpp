#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "treerightsidevaluesjson.h"
#include "propertynodejson.h"

TreeRightSideValuesJson::TreeRightSideValuesJson(const QJsonObject &_leftSide)
    : TreeRightSideValues(),
      m_json(_leftSide)
{}

QMap<QString, double> TreeRightSideValuesJson::values() const
{
    return m_values;
}

void TreeRightSideValuesJson::setValues(const QMap<QString, double> &_values)
{
    m_values = _values;
}

//! _treeName should contain extention
void TreeRightSideValuesJson::readValues(const QString &_treeName)
{
    m_openedFile = _treeName;
    m_values.clear();

    QFile loadFile(_treeName);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open" << _treeName;
        return;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject obj = loadDoc.object();
    m_values = extractValues(obj);
}

void TreeRightSideValuesJson::writeValues(const QString &_treeName)
{
    QFile saveFile(_treeName + extension());
    if (!saveFile.open(QIODevice::WriteOnly))
        qDebug() << Q_FUNC_INFO << "Couldn't open save file.";

    m_json = addValues(m_json);

    QJsonDocument saveDoc(m_json);
    saveFile.write(saveDoc.toJson());
}

//! joins m_values and m_json
QJsonObject TreeRightSideValuesJson::addValues(QJsonObject _obj)
{
    if(_obj.contains("key"))
    {
        QString key = _obj["key"].toString();
        if(m_values.contains(key))
        {
            double value = m_values[key];
            _obj["value"] = value;
        }
    }

    if(_obj.contains("nodes"))
    {
        QJsonArray arr = _obj["nodes"].toArray();
        for(int i = 0; i < arr.size(); ++i)
        {
            arr[i] = addValues(arr.at(i).toObject());
        }
        _obj["nodes"] = arr;
    }
    return _obj;
}

QMap<QString, double> TreeRightSideValuesJson::extractValues(const QJsonObject &_jObject) const
{
    QMap<QString, double> ans;
    if(_jObject.contains("value"))
    {
        QString key = _jObject["key"].toString();
        double  val = _jObject["value"].toDouble();
        if(key.isEmpty())
            qDebug() << Q_FUNC_INFO << "ERROR IN SOURSE DATA: key is empty in " + m_openedFile;
        else
            ans[key] = val;
    }

    if(_jObject.contains("nodes"))
    {
        QJsonArray arr = _jObject["nodes"].toArray();
        for(int i = 0; i < arr.size(); ++i)
        {
            ans.unite(extractValues(arr.at(i).toObject()));
        }
    }
    return ans;
}

QString TreeRightSideValuesJson::extension()
{
    return ".json";
}
