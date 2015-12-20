#ifndef PROPERYNODE_H
#define PROPERYNODE_H

#include <QString>
#include <QList>

class ProperyNode
{
public:
    explicit ProperyNode(const QString& _description, const QString& _key);
    ~ProperyNode();

    const QList<ProperyNode*> children() const;
    void setChildren(QList<ProperyNode*> _nodes);
    void addChild(ProperyNode* _node);
    void clearChilds();

    QString description() const;
    QString key() const;

private:
    QList<ProperyNode*> m_children;
    QString m_description;
    QString m_key;
};

#endif // PROPERYNODE_H
