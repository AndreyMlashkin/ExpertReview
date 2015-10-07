#ifndef PROPERYNODE_H
#define PROPERYNODE_H

#include <QObject>

class ProperyNode : public QObject
{
    Q_OBJECT
public:
    explicit ProperyNode(QObject *_parent = 0);
    ~ProperyNode();

    const QList<ProperyNode*> children() const;
    void setChildren(QList<ProperyNode*> _nodes);
    void addChild(ProperyNode* _node);
    void clearChilds();

    QString description() const;
    void setDescription(const QString& _desc);

private:
    QList<ProperyNode*> m_children;
    QString m_description;
};

#endif // PROPERYNODE_H
