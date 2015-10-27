#include "treeleftsideinfofactory.h"
#include "treeleftsideinfofile.h"
#include "treerightsidevaluesfile.h"

TreeLeftSideInfoFactory::TreeLeftSideInfoFactory()
{}

TreeLeftSideInfo *TreeLeftSideInfoFactory::getLeftSideInfo(const QString &_id)
{
    TreeLeftSideInfo* info = new TreeLeftSideInfoFile(_id);
    return info;
}
