#ifndef TREEINFOFACTORY_H
#define TREEINFOFACTORY_H

#include <QString>

class TreeLeftSideInfo;
class TreeRightSideValues;

class TreeInfoFactory
{
public:
    TreeInfoFactory();

    virtual TreeLeftSideInfo* getLeftSideInfo(const QString& _id) = 0;
    virtual TreeRightSideValues* getRightSideValues(const QString& _id) = 0;
};

#endif // TREEINFOFACTORY_H
