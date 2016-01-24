#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>

#include "treeleftsideinfojson.h"
#include "treerightsidevaluesjson.h"

TreeLeftSideInfoJson::TreeLeftSideInfoJson()
    : TreeLeftSideInfo(),
      m_isActual(false)
{}

TreeLeftSideInfoJson::TreeLeftSideInfoJson(const QString &_treeName)
    : TreeLeftSideInfo(),
      m_isActual(false)
{
    open(_treeName);
}

TreeLeftSideInfoJson::~TreeLeftSideInfoJson()
{
    clear();
}

void TreeLeftSideInfoJson::clear()
{
    m_isActual = false;
    qDeleteAll(m_nodes);
}

void TreeLeftSideInfoJson::open(const QString &_treeName)
{
    m_isActual = false;
    m_treeName = _treeName;

    QFile loadFile(_treeName + extension());
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open" << _treeName + extension() << "at" << Q_FUNC_INFO;
        return;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());
}

bool TreeLeftSideInfoJson::save() const
{
    QFile saveFile(treeName() + extension());

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Couldn't open save file" << Q_FUNC_INFO;
        return false;
    }

    QJsonObject leftSideObject;
    write(leftSideObject);
    QJsonDocument saveDoc(leftSideObject);
    saveFile.write(saveDoc.toJson());

    m_actualJson = leftSideObject;
    m_isActual = true;

    return true;
}

QString TreeLeftSideInfoJson::treeName() const
{
    return m_treeName;
}

QString TreeLeftSideInfoJson::name() const
{
    if(m_name.isEmpty())
        return TreeLeftSideInfo::name();
    return m_name;
}

const QList<PropertyNode *> TreeLeftSideInfoJson::nodes()
{
    return PropertyNodeJson::toBaseNodesList(m_nodes);
}

QStringList TreeLeftSideInfoJson::planeDescriptions() const
{
    return getPlaneListOfProperties(actualJson(), "description");
}

QStringList TreeLeftSideInfoJson::planeKeys() const
{
    return getPlaneListOfProperties(actualJson(), "key");
}

int TreeLeftSideInfoJson::savedRightSidesCount() const
{
    int count = 0;
    forever
    {
        QString path = rightSidePath(count);
        QFileInfo info(path);
        if(info.exists())
            ++count;
        else
            return count;
    }
}

QString TreeLeftSideInfoJson::defaultRightSideTreeName() const
{
    return m_defaultRightSideTreeName;
}

QStringList TreeLeftSideInfoJson::savedRightSidesTreeNames() const
{
    QStringList ans;
    int count = 0;
    forever
    {
        QString path = rightSidePath(count);
        QFileInfo info(path);
        if(info.exists())
        {
            ++count;
            ans << path;
        }
        else
            return ans;
    }
}

QString TreeLeftSideInfoJson::savedAverageRightSideTreeName() const
{
    return m_treeName + "_average" + extension();
}

TreeRightSideValues *TreeLeftSideInfoJson::createRightSide() const
{
    TreeRightSideValuesJson* rightSideValues = new TreeRightSideValuesJson(actualJson());
    return rightSideValues;
}

bool TreeLeftSideInfoJson::import(TreeLeftSideInfo *_otherInfo, ImportPolicy _policy)
{
    if(_policy == ResetOldData)
    {
        clear();
        m_treeName = _otherInfo->treeName();
    }

    m_nodes << PropertyNodeJson::fromBaseNodesList(_otherInfo->nodes());
    m_planeDescriptions << _otherInfo->planeDescriptions();
    m_planeKeys << _otherInfo->planeKeys();

    m_isActual = false;
    return true;
}

QStringList TreeLeftSideInfoJson::getPlaneListOfProperties(const QJsonObject &_json, const QString &_prop)
{
    QStringList ans;
    if(_json.contains(_prop))
    {
        QString desc = _json[_prop].toString();
        ans << desc;
    }

    if(_json.contains("nodes"))
    {
        QJsonArray arr = _json["nodes"].toArray();
        for(int i = 0; i < arr.size(); ++i)
            ans << getPlaneListOfProperties(arr.at(i).toObject(), _prop);
    }
    return ans;
}

QString TreeLeftSideInfoJson::extension()
{
    return QStringLiteral(".json");
}

QString TreeLeftSideInfoJson::rightSidePath(int _numer) const
{
    return m_treeName + QString::number(_numer) + extension();
}

void TreeLeftSideInfoJson::read(const QJsonObject &_json)
{
    m_isActual = false;

    clear(); // !!!

    m_name = _json["name"].toString();
    m_defaultRightSideTreeName = _json["defaultRightSideName"].toString();

    QJsonArray nodes = _json["nodes"].toArray();
    for (int i = 0; i < nodes.size(); ++i)
    {
        QJsonObject node = nodes[i].toObject();
        PropertyNodeJson* jNode = new PropertyNodeJson();
        jNode->read(node);
        m_nodes << jNode;
    }
}

void TreeLeftSideInfoJson::write(QJsonObject &json) const
{
    json["name"] = m_treeName;

    QJsonArray nodesArray;
    foreach (PropertyNodeJson* node, m_nodes)
    {
        QJsonObject nodeObject;
        node->write(nodeObject);
        nodesArray.append(nodeObject);
    }
    json["nodes"] = nodesArray;
}

QJsonObject &TreeLeftSideInfoJson::actualJson() const
{
    if(!m_isActual)
    {
        QJsonObject obj;
        write(obj);
        m_actualJson = obj;
    }
    return m_actualJson;
}
