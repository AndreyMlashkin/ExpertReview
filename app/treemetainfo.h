#ifndef TREEMETAINFO_H
#define TREEMETAINFO_H

#include <QList>
class ProperyNode;
class TreeMetaInfo
{
public:
    TreeMetaInfo();
    ~TreeMetaInfo();

    QList<ProperyNode*> createTestTree();
    const QList<ProperyNode*> nodes();

    void open(const QString& _filename);
    void save(const QString& _filename) const;

private:
    int calculateEnclosure(QString& _str);
    void clearNodes();

private:
    QString m_openedFile;
    QList<ProperyNode*> m_nodes;

};

#endif // TREEMETAINFO_H
