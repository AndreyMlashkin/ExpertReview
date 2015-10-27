#ifndef TREEINFOFACTORY_H
#define TREEINFOFACTORY_H

#include <QString>

class TreeLeftSideInfo;
class TreeRightSideValues;

class TreeLeftSideInfoFactory
{
public:
    TreeLeftSideInfoFactory();
    TreeLeftSideInfo* getLeftSideInfo(const QString& _id);
};

#endif // TREEINFOFACTORY_H
