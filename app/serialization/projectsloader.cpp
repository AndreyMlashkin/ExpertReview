#include <QDebug>
#include <QFileInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "projectsloader.h"
#include "projectapi.h"

ProjectsLoader::ProjectsLoader()
{

}

bool ProjectsLoader::load(const QFileInfo &fileInfo)
{
    // TODO здесь нужно грузить список файлов аналогично TreeLeftSideInfoJson::open(const QString &_treeName)
    // В случае если пустой, то? занести все файлы с расширением json?

    QFile loadFile(fileInfo.absoluteFilePath());
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open" << fileInfo.absoluteFilePath() << " at " << Q_FUNC_INFO;
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());
    m_opendProject = fileInfo;
    return true;
}

void ProjectsLoader::read(const QJsonObject &_json)
{
//    m_name = _json["name"].toString();

    m_loadedStructure.clear();
    QJsonArray nodes = _json["leftSides"].toArray();
    for (int i = 0; i < nodes.size(); ++i)
    {
        QJsonObject leftSide = nodes[i].toObject();
        QString leftSideName = leftSide["name"].toString();
        QVariantList varRightSideNames = leftSide["rightSides"].toArray().toVariantList();
        QStringList rightSideNames = toStringList(varRightSideNames);

        m_loadedStructure[leftSideName] = rightSideNames;
    }
}
