#ifndef PROPERTYNODEJSON_H
#define PROPERTYNODEJSON_H

#include <QJsonObject>
#include "nodesinfo/propertynode.h"

class PropertyNodeJson : public PropertyNode
{
public:
    PropertyNodeJson();
    PropertyNodeJson(const PropertyNode& _node);

    const QList<PropertyNode*> children() const override;
    void setChildren(QList<PropertyNode*> _nodes) override;
    void addChild(PropertyNode* _node) override;
    void clearChilds() override;

    void read(const QJsonObject &_json);
    void write(QJsonObject &json) const;

    static QList<PropertyNode*>     toBaseNodesList  (QList<PropertyNodeJson*> _jList);
    static QList<PropertyNodeJson*> fromBaseNodesList(QList<PropertyNode *> _jList);
private:
    QList<PropertyNodeJson*> m_jsonChildren;

};

#endif // PROPERTYNODEJSON_H
