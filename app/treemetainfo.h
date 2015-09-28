#ifndef TREEMETAINFO_H
#define TREEMETAINFO_H

#include <QList>
#include <QString>
class ProperyNode;
class TreeMetaInfo
{
public:
    TreeMetaInfo();
    ~TreeMetaInfo();

    QList<ProperyNode*> createTestTree();
    const QList<ProperyNode*> nodes();
    QStringList planeNodes();

    void open(const QString& _filename);
    void save(const QString& _filename) const;

private:
    int calculateEnclosure(QString& _str);
    void clearNodes();

private:
    QString m_openedFile;
    QList<ProperyNode*> m_nodes;
    QStringList m_planeNodes;
};

#endif // TREEMETAINFO_H
