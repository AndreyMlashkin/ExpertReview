#include "properynode.h"

ProperyNode::ProperyNode(const QString &_description, const QString &_key)
    : m_description(_description),
      m_key(_key)
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

QString ProperyNode::key() const
{
    return m_key;
}
