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

protected:
    PropertyNode() {}
    void setKey(const QString& _key);
    void setDescription(const QString& _description);

private:
    QList<PropertyNode*> m_children;
    QString m_description;
    QString m_key;
};

#endif // PROPERYNODE_H
