#include "treeinfofactory.h"
#include "treeleftsideinfofile.h"
#include "treerightsidevaluesfile.h"

TreeInfoFactory::TreeInfoFactory()
{
}

TreeLeftSideInfo *TreeInfoFactory::getLeftSideInfo(const QString &_id)
{
    TreeLeftSideInfo* info = new TreeLeftSideInfoFile();
    info->open(_id);
    return info;
}

TreeRightSideValues *TreeInfoFactory::getRightSideValues(const QString &_id)
{
    TreeRightSideValues* values = new TreeRightSideValuesFile();
    values->readValues(_id);
    return values;
}
