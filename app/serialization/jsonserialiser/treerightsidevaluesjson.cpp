#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "treerightsidevaluesjson.h"
#include "propertynodejson.h"

TreeRightSideValuesJson::TreeRightSideValuesJson(const QJsonObject &_leftSide)
    : TreeRightSideValues(),
      m_json(_leftSide)
{
    qDebug() << Q_FUNC_INFO; // TODO remove
}

TreeRightSideValuesJson::~TreeRightSideValuesJson()
{
    qDebug() << Q_FUNC_INFO << id(); // TODO remove
}

QMap<QString, double> TreeRightSideValuesJson::values() const
{
    if(m_values.isEmpty())
    {
        TreeRightSideValuesJson* nonConstThis = const_cast<TreeRightSideValuesJson*>(this);
        nonConstThis->m_values = nonConstThis->extractValues(m_json); // init all values with 0.
        Q_ASSERT(m_values.size() != 0);
    }

    return m_values;
}

void TreeRightSideValuesJson::setValues(const QMap<QString, double> &_values)
{
    m_values = _values;
}

void TreeRightSideValuesJson::readValues(const QString &_id)
{
    m_openedFile = _id;
    m_values.clear();

    QFile loadFile(_id);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open" << _id;
        return;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject obj = loadDoc.object();
    m_values = extractValues(obj);
}

void TreeRightSideValuesJson::writeValues(const QString &_id)
{    
    QFile saveFile(_id);
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
    if(_jObject.contains("key"))
    {
        QString key = _jObject["key"].toString();
        double  val = _jObject.contains("value") ?
                    _jObject["value"].toDouble()
                    : 0;

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
