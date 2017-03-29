#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>

#include "projectapi.h"
#include "treeleftsideinfojson.h"
#include "treerightsidevaluesjson.h"

TreeLeftSideInfoJson::TreeLeftSideInfoJson(const ProjectsLoaderPtr &_loader)
    : TreeLeftSideInfo(_loader),
      m_isActual(false)
{}

TreeLeftSideInfoJson::TreeLeftSideInfoJson(const QString &_treeName, const ProjectsLoaderPtr &_loader)
    : TreeLeftSideInfo(_loader),
      m_treeName(_treeName),
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

    QFile loadFile(getPath());
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open " << getPath() << "at" << Q_FUNC_INFO;
        return;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());

    // workawound to backward compatibility
    if(m_treeName.isEmpty())
    {
        qWarning() << "tree name param param was empty! Use " << _treeName;
        m_treeName = _treeName;
    }
}

bool TreeLeftSideInfoJson::save() const
{
    QFile saveFile(getPath());

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
    if(m_guiName.isEmpty())
        return TreeLeftSideInfo::name();
    return m_guiName;
}

QString TreeLeftSideInfoJson::getPath() const
{
    return m_loader->projectDir() + treeName();
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

QList<int> TreeLeftSideInfoJson::planeMinValues() const
{
    return toIntList(getPlaneListOfProperties(actualJson(), "minimum"));
}

QList<int> TreeLeftSideInfoJson::planeMaxValues() const
{
    return toIntList(getPlaneListOfProperties(actualJson(), "maximum"));
}

int TreeLeftSideInfoJson::savedRightSidesCount() const
{
    return m_loader->avaliableRightSides(m_treeName).size();
}

QString TreeLeftSideInfoJson::defaultRightSideTreeName() const
{
    return m_defaultRightSideTreeName;
}

QStringList TreeLeftSideInfoJson::savedRightSidesTreeNames() const
{
    return m_loader->avaliableRightSides(m_treeName);
}

QString TreeLeftSideInfoJson::savedAverageRightSideTreeName() const
{
    return m_treeName + "_average";
}

TreeRightSideValues *TreeLeftSideInfoJson::createRightSide() const
{
    TreeRightSideValuesJson* rightSideValues = new TreeRightSideValuesJson(m_treeName, actualJson());
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

    m_isActual = false;
    return true;
}

QStringList TreeLeftSideInfoJson::getPlaneListOfProperties(const QJsonObject &_json, const QString &_prop)
{
    QStringList ans;
    if(_json.contains(_prop))
    {
        ans << _json[_prop].toVariant().toString();
    }
    else
    {
        ans << QString(); // add a placeholder.
    }

    if(_json.contains("nodes"))
    {
        QJsonArray arr = _json["nodes"].toArray();
        for(int i = 0; i < arr.size(); ++i)
            ans << getPlaneListOfProperties(arr.at(i).toObject(), _prop);
    }
    return ans;
}

void TreeLeftSideInfoJson::read(const QJsonObject &_json)
{
    m_isActual = false;

    clear(); // !!!

    m_guiName = _json["guiName"].toString();
    m_treeName = _json["internalName"].toString();
    m_defaultRightSideTreeName = _json["defaultRightSideGuiName"].toString();

    if(m_treeName.isEmpty())
        qWarning() << Q_FUNC_INFO << QString("tree name is empty in ") + getPath();
    if(m_guiName.isEmpty())
        qWarning() << Q_FUNC_INFO << " gui name is empty in " << getPath();
    if(m_defaultRightSideTreeName.isEmpty())
        qInfo() << Q_FUNC_INFO << " defaultRightSideTreeName is empty in " << getPath();

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
    json["guiName"] = m_guiName;

    QJsonArray nodesArray;
    for (PropertyNodeJson* node : m_nodes)
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
