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
    TreeRightSideValues* values = createRightSide();
    QString saved = savedRightSidesIds().at(_num);
    values->readValues(saved);
    return values;

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

