#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "projectsloader.h"
#include "projectapi.h"

ProjectsLoader::ProjectsLoader()
{

}

ProjectsLoader::~ProjectsLoader()
{
    unload();
}

QStringList ProjectsLoader::avaliableLeftSides() const
{
    return m_loadedStructure.keys();
}

TreeLeftSideInfo *ProjectsLoader::getLeftSideInfo(const QString &_treeName)
{
    if(avaliableLeftSides().contains(_treeName))
        return m_factory.getLeftSideInfo(_treeName);
    else
        return nullptr;
}

bool ProjectsLoader::load(const QFileInfo &fileInfo)
{
    qDebug() << Q_FUNC_INFO;
    if(m_opendProject.exists() && !m_loadedStructure.isEmpty())
        unload();

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

    //TODO remove it
    tryCompatibilityFillStructure();

    return true;
}

bool ProjectsLoader::unload() const
{
    qDebug() << Q_FUNC_INFO;
    QFile saveFile(m_opendProject.absoluteFilePath());
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Couldn't open" << m_opendProject.absoluteFilePath() << " at " << Q_FUNC_INFO;
        return false;
    }
    QJsonObject projectStructure;
    write(projectStructure);

    QJsonDocument saveDoc(projectStructure);
    saveFile.write(saveDoc.toJson(QJsonDocument::Indented));

    return true;
}

void ProjectsLoader::read(const QJsonObject &_json)
{
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

void ProjectsLoader::write(QJsonObject &_json) const
{
    QMapIterator<QString, QStringList> iter(m_loadedStructure);

    while(iter.hasNext())
    {
        iter.next();
        _json[iter.key()] = QJsonArray::fromStringList(iter.value());
    }
}

void ProjectsLoader::tryCompatibilityFillStructure()
{
    qDebug() << Q_FUNC_INFO;
    QDir current = m_opendProject.absoluteDir();
    QStringList filters {"*.json"};
    QFileInfoList allFiles = current.entryInfoList(filters, QDir::NoDotAndDotDot | QDir::Files);

    qDebug() << Q_FUNC_INFO << current.absolutePath() << " " << allFiles.size();

    for(const QFileInfo& info : allFiles)
    {
        QString name = info.baseName();
        QChar lastChar = name.at(name.size() - 1);
        bool isRightSide = lastChar.isDigit();
        if(isRightSide)
        {
            QString rightSideName = name;
            rightSideName = rightSideName.remove(rightSideName.size() - 1, 1);
            m_loadedStructure[rightSideName].append(name);
        }
        else
        {
            if(!m_loadedStructure.contains(name))
                m_loadedStructure[name] = QStringList();
        }
    }
}
