#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QGuiApplication>

#include "projectapi.h"
#include "json_constants.h"
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

    QJsonParseError error;
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData, &error));

    if(loadDoc.isObject())
        read(loadDoc.object());
    else
    {
        qCritical() << "Error reading file "
                    << error.errorString() << "at " << error.offset;
        QGuiApplication::exit(-1);
    }

//    Q_ASSERT_X(m_treeName == _treeName, "Left side name is wrong ",
//               QString("expected %1, got %2").arg(_treeName)
//               .arg(m_treeName)
//               .toLocal8Bit());
    m_treeName = _treeName;
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

QString TreeLeftSideInfoJson::getTreeName() const
{
    return m_treeName;
}

QString TreeLeftSideInfoJson::getName() const
{
    if(m_guiName.isEmpty())
        return TreeLeftSideInfo::getName();
    return m_guiName;
}

QString TreeLeftSideInfoJson::getPath() const
{
    return m_loader->projectDir() + getTreeName();
}

const QList<PropertyNode *> TreeLeftSideInfoJson::nodes()
{
    return PropertyNodeJson::toBaseNodesList(m_nodes);
}

QStringList TreeLeftSideInfoJson::planeDescriptions() const
{
    return getPlaneListOfProperties(actualJson(), json_constants::description);
}

QStringList TreeLeftSideInfoJson::planeKeys() const
{
    return getPlaneListOfProperties(actualJson(), json_constants::key);
}

QList<int> TreeLeftSideInfoJson::planeMinValues() const
{
    return toIntList(getPlaneListOfProperties(actualJson(), json_constants::minimum));
}

QList<int> TreeLeftSideInfoJson::planeMaxValues() const
{
    return toIntList(getPlaneListOfProperties(actualJson(), json_constants::maximum));
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
        m_treeName = _otherInfo->getTreeName();
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

    if(_json.contains(json_constants::nodes))
    {
        QJsonArray arr = _json[json_constants::nodes].toArray();
        for(int i = 0; i < arr.size(); ++i)
            ans << getPlaneListOfProperties(arr.at(i).toObject(), _prop);
    }
    return ans;
}

void TreeLeftSideInfoJson::read(const QJsonObject &_json)
{
    m_isActual = false;

    clear(); // !!!

    m_guiName = _json[json_constants::guiName].toString();
    m_treeName = _json["internalName"].toString();
    m_defaultRightSideTreeName = _json["defaultRightSideGuiName"].toString();

    if(m_treeName.isEmpty())
        qWarning() << Q_FUNC_INFO << QString("tree name is empty in ") + getPath();
    if(m_guiName.isEmpty())
        qWarning() << Q_FUNC_INFO << " gui name is empty in " << getPath();
    if(m_defaultRightSideTreeName.isEmpty())
        qInfo() << Q_FUNC_INFO << " defaultRightSideTreeName is empty in " << getPath();

    QJsonArray nodes = _json[json_constants::nodes].toArray();
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
    json[json_constants::guiName] = m_guiName;

    QJsonArray nodesArray;
    for (PropertyNodeJson* node : m_nodes)
    {
        QJsonObject nodeObject;
        node->write(nodeObject);
        nodesArray.append(nodeObject);
    }
    json[json_constants::nodes] = nodesArray;
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
