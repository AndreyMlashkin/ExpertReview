#ifndef TREEINFOFACTORY_H
#define TREEINFOFACTORY_H

#include <QString>

class TreeLeftSideInfo;
class TreeRightSideValues;

class TreeInfoFactory
{
public:
    TreeInfoFactory();

    TreeLeftSideInfo* getLeftSideInfo(const QString& _id);
    TreeRightSideValues* getRightSideValues(const QString& _id = QString());
};

#endif // TREEINFOFACTORY_H
