#include <QDebug>
#include <QStringList>
#include <QStack>

#include "treeleftsideinfo.h"
#include "properynode.h"
#include "treerightsidevalues.h"

void operator <<(QStack<ProperyNode*>& _stack, const QList<ProperyNode*>& _list)
{
    foreach (ProperyNode* node, _list)
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

void TreeLeftSideInfo::import(TreeLeftSideInfo *)
{
    qDebug() << "Import function is not defined!";
}

void TreeLeftSideInfo::fillTitlesAndLeafs()
{
    QList<ProperyNode*> allNodes = nodes();
    QStack<ProperyNode*> stack;

    stack << allNodes;

    while(!stack.isEmpty())
    {
        ProperyNode* node = stack.pop();
        if(node->children().isEmpty())
            m_leafs << node->description();
        else
        {
            m_titles << node->description();
            stack << node->children();
        }
    }
}

