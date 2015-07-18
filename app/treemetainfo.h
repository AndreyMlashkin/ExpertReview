#ifndef TREEMETAINFO_H
#define TREEMETAINFO_H

#include <QList>
class ProperyNode;
class TreeMetaInfo
{
public:
    TreeMetaInfo();

    QList<ProperyNode*> createTestTree();

    void serialize();
    void deserialize();


};

#endif // TREEMETAINFO_H
