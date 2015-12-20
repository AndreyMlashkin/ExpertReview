#include <include/qjson/qobjecthelper.h>

#include "treeleftsidejsonfile.h"
#include "properynode.h"

TreeLeftSideJsonFile::TreeLeftSideJsonFile()
    : QObject(),
      TreeLeftSideInfo()
{
}

TreeLeftSideJsonFile::~TreeLeftSideJsonFile()
{

}

void TreeLeftSideJsonFile::open(const QString &_treeName)
{

}

QString TreeLeftSideJsonFile::id() const
{

}

const QList<ProperyNode *> TreeLeftSideJsonFile::nodes()
{

}

QStringList TreeLeftSideJsonFile::planeDescriptions() const
{

}

QStringList TreeLeftSideJsonFile::planeKeys() const
{

}

int TreeLeftSideJsonFile::savedRightSidesCount() const
{

}

QStringList TreeLeftSideJsonFile::savedRightSidesIds() const
{

}

QString TreeLeftSideJsonFile::savedAverageRightSideId() const
{

}

TreeRightSideValues *TreeLeftSideJsonFile::createRightSide() const
{

}

QVariantList TreeLeftSideJsonFile::serialiseNodes() const
{
    QVariantList ans;
    foreach(ProperyNode* node, m_nodes)
    {
        ans << QJson::QObjectHelper::qobject2qvariant(node);
    }
    return ans;
}

void TreeLeftSideJsonFile::deseraliseNodes(const QVariantList &_nodes)
{
    qDeleteAll(m_nodes);
/*    foreach (const QVariant& var, _nodes)
    {
        QJson::QObjectHelper::qvariant2qobject()) qvariant2qobject(var.toMap(), stream);
    }*/
}
