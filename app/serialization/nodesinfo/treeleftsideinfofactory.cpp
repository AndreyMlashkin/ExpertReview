#include <QCache>

#include "serialization/nodesinfo/treeleftsideinfofactory.h"
#include "serialization/jsonserialiser/treeleftsideinfojson.h"

class TreeLeftSideInfoFactory::TreeLeftSideInfoFactoryPrivate
{
public:
    TreeLeftSideInfoFactoryPrivate() {}
    ~TreeLeftSideInfoFactoryPrivate() {}

    TreeLeftSideInfo* getOrCreateLeftSideInfo(const QString &_treeName);

private:
    QCache<QString, TreeLeftSideInfo> m_leftSides;
};

TreeLeftSideInfo *TreeLeftSideInfoFactory::TreeLeftSideInfoFactoryPrivate::getOrCreateLeftSideInfo(const QString &_treeName)
{
    if(!m_leftSides.contains(_treeName))
    {
        TreeLeftSideInfoJson* leftSide = new TreeLeftSideInfoJson(_treeName);
        m_leftSides.insert(_treeName, leftSide);
    }
    return m_leftSides[_treeName];
}

// ----------------------------------------------------------------

QSharedPointer<TreeLeftSideInfoFactory::TreeLeftSideInfoFactoryPrivate> TreeLeftSideInfoFactory::m_pimpl
 = QSharedPointer<TreeLeftSideInfoFactory::TreeLeftSideInfoFactoryPrivate>();

TreeLeftSideInfoFactory::TreeLeftSideInfoFactory()
{}

TreeLeftSideInfo *TreeLeftSideInfoFactory::getLeftSideInfo(const QString &_treeName)
{
    return pimpl()->getOrCreateLeftSideInfo(_treeName);
}

TreeLeftSideInfoFactory::TreeLeftSideInfoFactoryPrivate *TreeLeftSideInfoFactory::pimpl()
{
    if(m_pimpl.isNull())
        m_pimpl = QSharedPointer<TreeLeftSideInfoFactory::TreeLeftSideInfoFactoryPrivate>
                            (new TreeLeftSideInfoFactory::TreeLeftSideInfoFactoryPrivate());

    return m_pimpl.data();
}

