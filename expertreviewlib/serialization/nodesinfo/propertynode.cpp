#include "propertynode.h"

PropertyNode::PropertyNode(const QString &_description, const QString &_key)
    : m_description(_description),
      m_key(_key)
{}

PropertyNode::~PropertyNode()
{
    clearChilds();
}

const QList<PropertyNode *> PropertyNode::children() const
{
    return m_children;
}

void PropertyNode::setChildren(QList<PropertyNode *> _nodes)
{
    clearChilds();
    m_children = _nodes;
}

void PropertyNode::addChild(PropertyNode *_node)
{
    m_children << _node;
}

void PropertyNode::clearChilds()
{
    for (PropertyNode* node : m_children)
        delete node;
    m_children = QList<PropertyNode*>();
}

QString PropertyNode::description() const
{
    return m_description;
}

QString PropertyNode::key() const
{
    return m_key;
}

void PropertyNode::setKey(const QString &_key)
{
    m_key = _key;
}

void PropertyNode::setDescription(const QString &_description)
{
    m_description = _description;
}
