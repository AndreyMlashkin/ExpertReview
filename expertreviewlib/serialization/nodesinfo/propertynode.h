#ifndef PROPERYNODE_H
#define PROPERYNODE_H

#include <QString>
#include <QList>

//!
//! \brief The PropertyNode class абстрактный элемент древовидной структуры проекта.
//!
class PropertyNode
{
public:
    explicit PropertyNode(const QString& _description, const QString& _key);
    virtual ~PropertyNode();

    virtual const QList<PropertyNode*> children() const;
    virtual void setChildren(QList<PropertyNode*> _nodes);
    virtual void addChild(PropertyNode* _node);
    virtual void clearChilds();

    QString description() const;
    QString key() const;
    int minValue() const   { return m_minValue; }
    int maxValue() const   { return m_maxValue; }

protected:
    PropertyNode() {}
    void setKey(const QString& _key);
    void setDescription(const QString& _description);
    void setMax(int _newValue) { m_maxValue = _newValue; }
    void setMin(int _newValue) { m_minValue = _newValue; }

private:
    QList<PropertyNode*> m_children;
    QString m_description;
    QString m_key;
    int m_minValue;
    int m_maxValue;
};

#endif // PROPERYNODE_H
