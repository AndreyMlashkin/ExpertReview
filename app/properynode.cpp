#include "properynode.h"

ProperyNode::ProperyNode(QObject *_parent)
    : QObject(_parent)
{}

ProperyNode::~ProperyNode()
{
    clearChilds();
}

const QList<ProperyNode *> ProperyNode::children() const
{
    return m_children;
}

void ProperyNode::setChildren(QList<ProperyNode *> _nodes)
{
    clearChilds();
    m_children = _nodes;
}

void ProperyNode::addChild(ProperyNode *_node)
{
    m_children << _node;
}

void ProperyNode::clearChilds()
{
    foreach (ProperyNode* node, m_children)
        delete node;
    m_children = QList<ProperyNode*>();
}

QString ProperyNode::description() const
{
    return m_description;
}

void ProperyNode::setDescription(const QString &_desc)
{
    m_description = _desc;
}
