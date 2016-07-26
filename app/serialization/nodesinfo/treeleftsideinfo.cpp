#include <QDebug>
#include <QStringList>
#include <QStack>

#include "treeleftsideinfo.h"
#include "propertynode.h"
#include "treerightsidevalues.h"

void operator <<(QStack<PropertyNode*>& _stack, const QList<PropertyNode*>& _list)
{
    foreach (PropertyNode* node, _list)
        _stack.append(node);
}

TreeLeftSideInfo::TreeLeftSideInfo(const ProjectsLoaderPtr &_loader)
    : m_loader(_loader)
{}

QStringList TreeLeftSideInfo::titles() const
{
    if(m_titles.isEmpty())
        const_cast<TreeLeftSideInfo*>(this)->fillTitlesAndLeafs();

    return m_titles;
}

QStringList TreeLeftSideInfo::leafs() const
{
    if(m_leafs.isEmpty())
        const_cast<TreeLeftSideInfo*>(this)->fillTitlesAndLeafs();

    return m_leafs;
}

TreeRightSideValues *TreeLeftSideInfo::openRightSide(int _num)
{
    Q_ASSERT(_num < savedRightSidesCount());
    QString saved = savedRightSidesTreeNames().at(_num);
    return m_loader->getOrCreateRightSide(treeName(), saved);
}

QList<TreeRightSideValues *> TreeLeftSideInfo::getLeftSides() const
{
    QString selfName = treeName();
    QStringList rightSides = m_loader->avaliableRightSides(selfName);
    QList<TreeRightSideValues *> result;

    for(const QString& rightSide : rightSides)
        result << m_loader->getOrCreateRightSide(selfName, rightSide, false);
    return result;
}

bool TreeLeftSideInfo::import(TreeLeftSideInfo *, ImportPolicy)
{
    qDebug() << Q_FUNC_INFO << "don't applied";
    return false;
}

void TreeLeftSideInfo::fillTitlesAndLeafs()
{
    QList<PropertyNode*> allNodes = nodes();
    QStack<PropertyNode*> stack;

    stack << allNodes;

    while(!stack.isEmpty())
    {
        PropertyNode* node = stack.pop();
        if(node->children().isEmpty())
            m_leafs << node->description();
        else
        {
            m_titles << node->description();
            stack << node->children();
        }
    }
}

