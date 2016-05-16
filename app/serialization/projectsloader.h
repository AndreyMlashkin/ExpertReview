#ifndef PROJECTSLOADER_H
#define PROJECTSLOADER_H

#include <QObject>
#include <QFileInfo>

class QJsonObject;

class ProjectsLoader : public QObject
{
public:
    ProjectsLoader();

    //! \brief load loads all left and right sides from project file, specified in fileInfo
    bool load(const QFileInfo& fileInfo);

private:
    void read(const QJsonObject &_json);

private:
    QFileInfo m_opendProject;
    //! Maps left side file names to it's right sides
    QMap<QString, QStringList> m_loadedStructure;
};

#endif // PROJECTSLOADER_H
