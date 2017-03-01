#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "projectsloader.h"
#include "projectapi.h"
#include "serialization/jsonserialiser/treeleftsideinfojson.h"
#include "serialization/jsonserialiser/treerightsidevaluesjson.h"

ProjectsLoader::ProjectsLoader()
    : m_self(this)
{}

ProjectsLoader::~ProjectsLoader()
{
    unload();
    clear();
}

QStringList ProjectsLoader::avaliableLeftSides() const
{
    return m_loadedStructure.keys();
}

void ProjectsLoader::loadAll()
{
    for(const QString& leftSide : m_loadedStructure.keys())
    {
        loadAllRightSides(leftSide);
    }
}

void ProjectsLoader::loadAllRightSides(const QString &_leftSideId)
{
    QStringList avaliableRightSides = m_loadedStructure[_leftSideId];
    for(const QString& rightSideName : avaliableRightSides)
    {
        createRightSide(_leftSideId, rightSideName);
    }
}

TreeLeftSideInfo *ProjectsLoader::getLeftSideInfo(const QString &_leftSideId)
{
    qDebug() << Q_FUNC_INFO << " " << _leftSideId << "\n"
             << m_loadedStructure.keys();
    if(avaliableLeftSides().contains(_leftSideId))
    {
        if(!m_leftSides.contains(_leftSideId))
            createLeftSide(_leftSideId);

        return m_leftSides[_leftSideId];
    }
    else
        return nullptr;
}

QStringList ProjectsLoader::avaliableRightSides(const QString _leftSideId) const
{
    auto rightSideInternalNames = m_loadedStructure[_leftSideId];
    QStringList res;
    for(const QString internalName : rightSideInternalNames)
    {
        res << internalName;
    }
    return res;
}

//!
//! \brief ProjectsLoader::getRightSide
//! \param _leftSideId
//! \param _rightSideId
//! \return can return null as result.
//!
TreeRightSideValues *ProjectsLoader::getRightSide(const QString &_leftSideId,
                                                  const QString &_rightSideId) const
{
    QPair<QString, QString> key(_rightSideId, _leftSideId);
    qDebug() << Q_FUNC_INFO << " " << m_rightSides.size();
    if(m_rightSides.contains(key))
        return m_rightSides[key];
    return nullptr;
}

TreeRightSideValues *ProjectsLoader::createRightSide(const QString &_leftSideId, bool _isTemp, bool _readOldValues)
{
    QString rightSideName = generateRightSideInternalName(_leftSideId);
    return createRightSide(_leftSideId, rightSideName, _isTemp, _readOldValues);
}

TreeRightSideValues *ProjectsLoader::createRightSide(const QString &_leftSideId,
                                                     const QString &_rightSideId,
                                                     bool _isTemp, bool _readOldValues)
{
    QPair<QString, QString> key(_rightSideId, _leftSideId);
    auto leftSide = getLeftSideInfo(_leftSideId);
    Q_ASSERT_X(leftSide, QString("left side with the name %1 does not exist!")
               .arg(_leftSideId).toLatin1(), "");
    TreeRightSideValues* rSide = leftSide->createRightSide();
    rSide->setId(_rightSideId);
    rSide->setTemp(_isTemp);

    if(_readOldValues)
    {
        QString path = projectDir() + _rightSideId;
        rSide->readValues(path);        
    }

    Q_ASSERT(!rSide->values().isEmpty());
    m_rightSides.insert(key, rSide);

    if(!_isTemp && !m_loadedStructure[_leftSideId].contains(_rightSideId))
        m_loadedStructure[_leftSideId] << _rightSideId;

    if(rSide->guiName().isEmpty())
    {
        QString guiName = leftSide->name() + QString::number(leftSide->getRightSides().count());
        rSide->setGuiName(guiName);
    }

    return rSide;
}

TreeRightSideValues *ProjectsLoader::getOrCreateRightSide(const QString &_leftSideId,
                                                          const QString &_rightSideId,
                                                          bool isTemp)
{
    TreeRightSideValues* result = getRightSide(_leftSideId, _rightSideId);
    if(result)
        return result;

    result = createRightSide(_leftSideId, _rightSideId, isTemp);
    return result;
}

void ProjectsLoader::removeRightSide(const QString &_rightSideId)
{
    for(auto iter = m_loadedStructure.begin(); iter != m_loadedStructure.end(); ++iter)
    {
        QStringList& rSides = iter.value();
        for(const QString& rSide : rSides)
        {
            if(rSide == _rightSideId)
            {
                rSides.removeOne(rSide);
                QString leftSideId = iter.key();
                QPair<QString, QString> key(rSide, leftSideId);
                m_rightSides.remove(key);
            }
        }
    }
}

bool ProjectsLoader::load(const QFileInfo &fileInfo)
{
    qDebug() << Q_FUNC_INFO << " loading " << fileInfo.absoluteFilePath();
    if(m_opendProject.exists() && !m_loadedStructure.isEmpty())
        unload();
    clear();

    QFile loadFile(fileInfo.absoluteFilePath());
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open" << fileInfo.absoluteFilePath() << " at " << Q_FUNC_INFO;
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonParseError error;
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData, &error));

    if(loadDoc.isObject())
        read(loadDoc.object());
    else
        qDebug() << Q_FUNC_INFO << " smth went wrong. " << error.errorString() << "at " << error.offset;

    m_opendProject = fileInfo;

    //TODO remove it
    //tryCompatibilityFillStructure();

    return true;
}

bool ProjectsLoader::unload() const
{
    qDebug() << Q_FUNC_INFO;
    bool success = unloadProjectStructure();
    Q_ASSERT(success);
    success = unloadRightSides();
    Q_ASSERT(success);
    return true;
}

bool ProjectsLoader::unloadProjectStructure() const
{
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

bool ProjectsLoader::unloadRightSides() const
{
    QStringList leftSides = avaliableLeftSides();
    for(const QString& leftSideName : leftSides)
    {
        QStringList rSides = avaliableRightSides(leftSideName);
        for(const QString& rSide : rSides)
        {
            TreeRightSideValues* rSideObj = getRightSide(leftSideName, rSide);
            if(rSideObj && !rSideObj->isTemp()) // rSideObj can be null, due to lazy initialisation.
            {
                // TODO id can be not unique
                QString path = projectDir() + rSideObj->id();
                rSideObj->writeValues(path);
            }
        }
    }
    return true;
}

void ProjectsLoader::clear()
{
    m_loadedStructure.clear();
    m_opendProject = QFileInfo();
    m_loadedStructure.clear();
    m_leftSides.clear();
    m_rightSides.clear();
}

void ProjectsLoader::read(const QJsonObject &_json)
{
    Q_ASSERT(m_loadedStructure.size() == 0);
    QJsonObject leftSides = _json["leftSides"].toObject();
    auto iter = leftSides.begin();

    while(iter != leftSides.end())
    {
        QString leftSideName = iter.key();
        QJsonArray rightSides = iter.value().toArray();

        m_loadedStructure[leftSideName] = QStringList();
        for(const auto& rightSide : rightSides) // TODO is duplicate check neded?
            m_loadedStructure[leftSideName] << rightSide.toString();
        ++iter;
    }
    qDebug() << Q_FUNC_INFO << "loaded " << m_loadedStructure;
}

void ProjectsLoader::write(QJsonObject &_json) const
{
    QJsonObject leftSides;
    auto iter = m_loadedStructure.begin();

    while(iter != m_loadedStructure.end())
    {
        Q_ASSERT(iter.key() != "tmp");
        leftSides[iter.key()] = QJsonArray::fromStringList(iter.value());
        ++iter;
    }
    _json["leftSides"] = leftSides;
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
        if(name.contains("average"))
            continue;

        QChar lastChar = name.at(name.size() - 1);
        bool isRightSide = lastChar.isDigit();
        if(isRightSide)
        {
            QString rightSideName = name;
            rightSideName = rightSideName.remove(rightSideName.size() - 1, 1);
            m_loadedStructure[rightSideName] << name; // TODO need check duplicate?
        }
        else
        {
            if(!m_loadedStructure.contains(name))
                m_loadedStructure[name] = QStringList();
        }
    }
}

void ProjectsLoader::createLeftSide(const QString &_treeName)
{
    TreeLeftSideInfoJson* leftSide = new TreeLeftSideInfoJson(_treeName, m_self);
    m_leftSides.insert(_treeName, leftSide);
}

QString ProjectsLoader::generateRightSideInternalName(const QString &_leftSide) const
{
    QStringList rightSides = avaliableRightSides(_leftSide);
    QString rightSideName;
    int suffix = rightSides.size();
    while(true)
    {
        rightSideName = _leftSide + QString::number(suffix);
        if(rightSides.contains(rightSideName))
            ++suffix;
        else
            break;
    }
    return rightSideName;
}

QString ProjectsLoader::projectDir() const
{
    return QDir::fromNativeSeparators(m_opendProject.absolutePath() + QDir::separator());
}
