#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "treeleftsideinfojson.h"

TreeLeftSideInfoJson::TreeLeftSideInfoJson()
    : TreeLeftSideInfo()
{}

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

}

QStringList TreeLeftSideInfoJson::savedRightSidesIds() const
{

}

QString TreeLeftSideInfoJson::savedAverageRightSideTreeName() const
{

}

TreeRightSideValues *TreeLeftSideInfoJson::createRightSide() const
{
    return nullptr;
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

void TreeLeftSideInfoJson::read(const QJsonObject &_json)
{

//    mPlayer.read(json["player"].toObject());

//    mLevels.clear();
//    QJsonArray levelArray = json["levels"].toArray();
//    for (int levelIndex = 0; levelIndex < levelArray.size(); ++levelIndex) {
//        QJsonObject levelObject = levelArray[levelIndex].toObject();
//        Level level;
//        level.read(levelObject);
//        mLevels.append(level);
//    }
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
