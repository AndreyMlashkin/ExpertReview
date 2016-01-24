#ifndef TREEINFOFACTORY_H
#define TREEINFOFACTORY_H

#include <QString>
#include <QSharedPointer>

class TreeLeftSideInfo;

class TreeLeftSideInfoFactory
{
public:
    TreeLeftSideInfoFactory();
    TreeLeftSideInfo* getLeftSideInfo(const QString& _treeName);

private:
    class TreeLeftSideInfoFactoryPrivate;
    TreeLeftSideInfoFactoryPrivate* pimpl();

    static QSharedPointer<TreeLeftSideInfoFactoryPrivate> m_pimpl;
};

#endif // TREEINFOFACTORY_H
