#ifndef PROJECTSLOADER_H
#define PROJECTSLOADER_H

#include <QObject>
#include <QFileInfo>
#include "serialization/nodesinfo/treeleftsideinfofactory.h"

class QJsonObject;
class TreeLeftSideInfo;

class ProjectsLoader : public QObject
{
public:
     ProjectsLoader();
    ~ProjectsLoader();

     QStringList avaliableLeftSides() const;
     TreeLeftSideInfo* getLeftSideInfo(const QString& _treeName);

public slots:
    //! \brief load loads all left and right sides from project file, specified in fileInfo
    bool load(const QFileInfo& fileInfo);
    bool unload() const;

private:
    void read(const QJsonObject &_json);
    void write(QJsonObject &_json) const;

    void tryCompatibilityFillStructure();

private:
    QFileInfo m_opendProject;
    //! Maps left side file names to it's right sides
    QMap<QString, QStringList> m_loadedStructure;

    TreeLeftSideInfoFactory m_factory;
};

#endif // PROJECTSLOADER_H