#include <QJsonArray>
#include "propertynodejson.h"
#include "json_constants.h"

PropertyNodeJson::PropertyNodeJson()
    : PropertyNode()
{}

PropertyNodeJson::PropertyNodeJson(const PropertyNode &_node)
    : PropertyNode()
{
    setKey(_node.key());
    setDescription(_node.description());
    setChildren(_node.children());
}

const QList<PropertyNode *> PropertyNodeJson::children() const
{
    return toBaseNodesList(m_jsonChildren);
}

void PropertyNodeJson::setChildren(QList<PropertyNode *> _nodes)
{
    for (PropertyNode* node : _nodes)
        addChild(node);
}

void PropertyNodeJson::addChild(PropertyNode *_node)
{
    Q_ASSERT(_node);
    PropertyNodeJson* jNode = dynamic_cast<PropertyNodeJson*>(_node);
    if(!jNode)
        jNode = new PropertyNodeJson(*_node);
    m_jsonChildren << jNode;
}

void PropertyNodeJson::clearChilds()
{
    PropertyNode::clearChilds();
    for (PropertyNodeJson* node : m_jsonChildren)
    {
        node->clearChilds();
        qDeleteAll(m_jsonChildren);
    }
}

void PropertyNodeJson::read(const QJsonObject &_json)
{
    clearChilds();

    setKey(_json[json_constants::key].toString());
    setDescription(_json["description"].toString());

    setMin(_json[json_constants::minimum].toString().toInt());
    int newMaxValue = (_json[json_constants::maximum].toString().toInt());
    newMaxValue = newMaxValue? newMaxValue : INT32_MAX;
    setMax(newMaxValue);

    QJsonArray childs = _json[json_constants::nodes].toArray();
    for(int i = 0; i < childs.size(); ++i)
    {
        QJsonObject node = childs.at(i).toObject();
        PropertyNodeJson* jNode = new PropertyNodeJson();
        jNode->read(node);
        m_jsonChildren << jNode;
    }
}

void PropertyNodeJson::write(QJsonObject &_json) const
{
    _json[json_constants::key]         = key();
    _json["description"] = description();
    _json[json_constants::minimum]     = minValue();
    _json[json_constants::maximum]     = maxValue();

    if(m_jsonChildren.size())
    {
        QJsonArray childs;
        for (PropertyNodeJson* child : m_jsonChildren)
        {
            QJsonObject obj;
            child->write(obj);
            childs.append(obj);
        }
        _json[json_constants::nodes] = childs;
    }
}

QList<PropertyNode *> PropertyNodeJson::toBaseNodesList(QList<PropertyNodeJson *> _jList)
{
    QList<PropertyNode *> ans;
    for (PropertyNodeJson* jNode : _jList)
        ans << jNode;
    return ans;
}

QList<PropertyNodeJson *> PropertyNodeJson::fromBaseNodesList(QList<PropertyNode *> _jList)
{
    QList<PropertyNodeJson *> ans;
    for (PropertyNode* node : _jList)
    {
        PropertyNodeJson* jNode = dynamic_cast<PropertyNodeJson*>(node);
        if(!jNode)
            jNode = new PropertyNodeJson(*node);
        ans << jNode;
    }
    return ans;
}
