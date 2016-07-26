#ifndef TREELEFTSIDEINFO_H
#define TREELEFTSIDEINFO_H

#include <QStringList>
#include "serialization/projectsloader.h"

class PropertyNode;
class TreeRightSideValues;
class ProjectsLoader;

class TreeLeftSideInfo
{
public:
    TreeLeftSideInfo(const ProjectsLoaderPtr& _loader);
    virtual ~TreeLeftSideInfo() {}
    virtual void clear() {} // !!! todo abstract

    virtual void open(const QString& _treeName) = 0;
    virtual QString treeName() const = 0;
    virtual QString name() const { return treeName(); }

    virtual const QList<PropertyNode*> nodes() = 0;
    virtual QStringList planeDescriptions() const = 0;
    virtual QStringList planeKeys() const = 0;

    QStringList titles() const;
    QStringList leafs() const;

    virtual int savedRightSidesCount() const = 0;
    virtual QString defaultRightSideTreeName() const = 0;
    virtual QStringList savedRightSidesTreeNames() const = 0;
    virtual QString savedAverageRightSideTreeName() const = 0;
    virtual TreeRightSideValues* createRightSide() const = 0;
    TreeRightSideValues* openRightSide(int _num);
    QList<TreeRightSideValues*> getLeftSides() const;

    enum ImportPolicy { AddData, ResetOldData };
    virtual bool import(TreeLeftSideInfo* _otherInfo, ImportPolicy _policy = ResetOldData);

private:
    void fillTitlesAndLeafs();

protected:
    ProjectsLoaderPtr m_loader;

private:
    mutable QStringList m_titles;
    mutable QStringList m_leafs;
};

#endif // TREELEFTSIDEINFO_H
