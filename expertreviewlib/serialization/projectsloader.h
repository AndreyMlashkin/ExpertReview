#ifndef PROJECTSLOADER_H
#define PROJECTSLOADER_H

#include <QPointer>
#include <QObject>
#include <QFileInfo>
#include <QMap>
#include <QSet>
#include <QCache>
#include "../expertreviewlib/export.h"

class QJsonObject;
class TreeLeftSideInfo;
class TreeRightSideValues;
class ProjectsLoader;
typedef QPointer<ProjectsLoader> ProjectsLoaderPtr;

//!
//! \brief The ProjectsLoader class класс программного интерфейса для загрузки и сохранения
//! векторов значений проектов и метаописания проектов.
//!
class EXPERTREVIEW_EXPORT ProjectsLoader : public QObject
{
public:
     ProjectsLoader();
    ~ProjectsLoader();
     QString projectDir() const;

     QStringList avaliableLeftSides() const;
     QMap<QString, QStringList> loadedStructure() { return m_loadedStructure; } // TODO hide it     
     void loadAll();
     void loadAllRightSides(const QString& _leftSideId);

     TreeLeftSideInfo* getLeftSideInfo(const QString& _leftSideId);

     QStringList avaliableRightSides(const QString _leftSideId) const;

     TreeRightSideValues* getRightSide(const QString& _leftSideId,
                                       const QString& _rightSideId) const;

     TreeRightSideValues* createRightSide(const QString& _leftSideId,
                                          bool isTemp = false);
     TreeRightSideValues* createRightSide(const QString& _leftSideId,
                                          const QString& _rightSideId,
                                          bool _isTemp = false);

     TreeRightSideValues* getOrCreateRightSide(const QString& _leftSideId,
                                               const QString& _rightSideId,
                                               bool isTemp = false);

     void removeRightSide(const QString& _rightSideId);

     inline ProjectsLoaderPtr getSelf() const { return m_self; }

public slots:
    //! \brief load loads all left and right sides from project file, specified in fileInfo
    bool load(const QFileInfo& fileInfo);
    bool unload() const;
    bool unloadProjectStructure() const;
    bool unloadRightSides() const;

private:
    void read(const QJsonObject &_json);
    void write(QJsonObject &_json) const;
    void clear();

    void tryCompatibilityFillStructure();
    void createLeftSide(const QString& _treeName);

    QString generateRightSideInternalName(const QString& _leftSide) const;

private:
    ProjectsLoaderPtr m_self;

    QFileInfo m_opendProject;
    //! Maps left side file names to it's right sides
    QMap<QString, QStringList> m_loadedStructure;

    QCache<QString, TreeLeftSideInfo> m_leftSides;
    QCache<QPair<QString, QString>, TreeRightSideValues> m_rightSides; // maps a pair of right side + left side to Right side object
};

#endif // PROJECTSLOADER_H
