#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>

#include "treeleftsideinfojson.h"
#include "treerightsidevaluesjson.h"

TreeLeftSideInfoJson::TreeLeftSideInfoJson()
    : TreeLeftSideInfo()
{}

TreeLeftSideInfoJson::TreeLeftSideInfoJson(const QString &_treeName)
    : TreeLeftSideInfo()
{
    open(_treeName);
}

TreeLeftSideInfoJson::~TreeLeftSideInfoJson()
{
    clear();
}

void TreeLeftSideInfoJson::clear()
{
    qDeleteAll(m_nodes);
}

void TreeLeftSideInfoJson::open(const QString &_treeName)
{
    m_treeName = _treeName;

    QFile loadFile(_treeName + extension());
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open" << _treeName + extension();
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
        qDebug() << Q_FUNC_INFO << "Couldn't open save file.";
        return false;
    }

    QJsonObject leftSideObject;
    write(leftSideObject);
    QJsonDocument saveDoc(leftSideObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

QString TreeLeftSideInfoJson::treeName() const
{
    return m_treeName;
}

const QList<PropertyNode *> TreeLeftSideInfoJson::nodes()
{
    return PropertyNodeJson::toBaseNodesList(m_nodes);
}

QStringList TreeLeftSideInfoJson::planeDescriptions() const
{
    return m_planeDescriptions;
}

QStringList TreeLeftSideInfoJson::planeKeys() const
{
    return m_planeKeys;
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
    return m_treeName + "Average" + extension();
}

TreeRightSideValues *TreeLeftSideInfoJson::createRightSide() const
{
    // TODO cache it
    QJsonObject selfJson;
    write(selfJson);

    TreeRightSideValuesJson* rightSideValues = new TreeRightSideValuesJson(selfJson);
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
    return true;
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
    clear();

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
