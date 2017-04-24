#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QGuiApplication>

#include "json_constants.h"
#include "treerightsidevaluesjson.h"
#include "propertynodejson.h"

TreeRightSideValuesJson::TreeRightSideValuesJson(const QString &_treeName, const QJsonObject &_leftSide)
    : TreeRightSideValues(),
      m_json(_leftSide),
      m_values(),
      m_treeName(_treeName),
      m_guiName()
{
    Q_ASSERT_X(!m_treeName.isEmpty(), "internalName should be filled also for right sides", "");
    qDebug() << Q_FUNC_INFO; // TODO remove
}

TreeRightSideValuesJson::~TreeRightSideValuesJson()
{
    qDebug() << Q_FUNC_INFO << id(); // TODO remove
}

QString TreeRightSideValuesJson::leftSideId() const
{
    return m_treeName;
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
    QJsonParseError error;
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData, &error));

    if(!loadDoc.isObject())
    {
        qCritical() << "Error reading file "
                    << error.errorString() << "at " << error.offset;
        QGuiApplication::exit(-1);
    }

    QJsonObject obj = loadDoc.object();
    m_values = extractValues(obj);

    if(obj.contains(json_constants::guiName))
        setGuiName(obj[json_constants::guiName].toString());
    else
        setGuiName(m_treeName);
}

void TreeRightSideValuesJson::writeValues(const QString &_id)
{    
    QFile saveFile(_id);
    if (!saveFile.open(QIODevice::WriteOnly))
        qDebug() << Q_FUNC_INFO << "Couldn't open save file.";

    m_json = addValues(m_json);
    m_json[json_constants::guiName] = m_guiName;

    QJsonDocument saveDoc(m_json);
    saveFile.write(saveDoc.toJson());
}

QString TreeRightSideValuesJson::guiName() const
{
    return m_guiName;
}

void TreeRightSideValuesJson::setGuiName(const QString &_guiName)
{
    m_guiName = _guiName;
}

//! joins m_values and m_json
QJsonObject TreeRightSideValuesJson::addValues(QJsonObject _obj)
{
    if(_obj.contains(json_constants::key))
    {
        QString key = _obj[json_constants::key].toString();
        if(m_values.contains(key))
        {
            double value = m_values[key];
            _obj[json_constants::value] = value;
        }
    }

    if(_obj.contains(json_constants::nodes))
    {
        QJsonArray arr = _obj[json_constants::nodes].toArray();
        for(int i = 0; i < arr.size(); ++i)
        {
            arr[i] = addValues(arr.at(i).toObject());
        }
        _obj[json_constants::nodes] = arr;
    }
    return _obj;
}

QMap<QString, double> extractValuesFromJObject(const QJsonObject &_jObject)
{
    QMap<QString, double> ans;
    if(_jObject.contains(json_constants::key))
    {
        QString key = _jObject[json_constants::key].toString();
        double  val = _jObject.contains(json_constants::value) ?
                    _jObject[json_constants::value].toDouble()
                    : 0;

        if(key.isEmpty())
            qDebug() << Q_FUNC_INFO << " ERROR IN SOURSE DATA: key is empty";
        else
            ans[key] = val;
    }

    if(_jObject.contains("nodes"))
    {
        QJsonArray arr = _jObject["nodes"].toArray();
        for(int i = 0; i < arr.size(); ++i)
        {
            ans.unite(extractValuesFromJObject(arr.at(i).toObject()));
        }
    }
    return ans;
}


QMap<QString, double> TreeRightSideValuesJson::extractValues(const QJsonObject &_jObject) const
{
    QMap<QString, double> ans;
    return extractValuesFromJObject(_jObject);
}
