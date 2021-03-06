#ifndef TREELEFTSIDEINFO_H
#define TREELEFTSIDEINFO_H

#include <QStringList>
#include "serialization/projectsloader.h"

class PropertyNode;
class TreeRightSideValues;
class ProjectsLoader;

//!
//! \brief The TreeLeftSideInfo class абстрактный класс для описания метаинформации проектов.
//!
class TreeLeftSideInfo
{
    friend class ProjectsLoader;
public:
    TreeLeftSideInfo(const ProjectsLoaderPtr& _loader);
    virtual ~TreeLeftSideInfo() {}
    virtual void clear() {} // !!! todo abstract

    virtual void open(const QString& _treeName) = 0;
    virtual QString getTreeName() const = 0;
    virtual QString getName() const { return getTreeName(); }

    virtual const QList<PropertyNode*> nodes() = 0;
    virtual QStringList planeDescriptions() const = 0;
    virtual QStringList planeKeys() const = 0;

    virtual QList<int> planeMinValues() const = 0;
    virtual QList<int> planeMaxValues() const = 0;

    QStringList titles() const;
    QStringList leafs() const;

    virtual int savedRightSidesCount() const = 0;
    virtual QString defaultRightSideTreeName() const = 0;
    virtual QStringList savedRightSidesTreeNames() const = 0;
    virtual QString savedAverageRightSideTreeName() const = 0;
    TreeRightSideValues* openRightSide(int _num);
    QList<TreeRightSideValues*> getRightSides() const;

    enum ImportPolicy { AddData, ResetOldData };
    virtual bool import(TreeLeftSideInfo* _otherInfo, ImportPolicy _policy = ResetOldData);

    virtual TreeRightSideValues* createRightSide() const = 0;
private:
    void fillTitlesAndLeafs();

protected:
    ProjectsLoaderPtr m_loader;

private:
    mutable QStringList m_titles;
    mutable QStringList m_leafs;
};

#endif // TREELEFTSIDEINFO_H
