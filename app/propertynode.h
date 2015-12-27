#ifndef PROPERYNODE_H
#define PROPERYNODE_H

#include <QObject>

class PropertyNode : public QObject
{
    Q_OBJECT
public:
    explicit PropertyNode(const QString& _description, const QString& _key);
    ~PropertyNode();

    const QList<PropertyNode*> children() const;
    void setChildren(QList<PropertyNode*> _nodes);
    void addChild(PropertyNode* _node);
    void clearChilds();

    QString description() const;
    QString key() const;

private:
    QList<PropertyNode*> m_children;
    QString m_description;
    QString m_key;
};

#endif // PROPERYNODE_H
