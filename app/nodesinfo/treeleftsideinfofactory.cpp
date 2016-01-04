#include "treeleftsideinfofactory.h"
#include "jsonserialiser/treeleftsideinfojson.h"

TreeLeftSideInfoFactory::TreeLeftSideInfoFactory()
{}

TreeLeftSideInfo *TreeLeftSideInfoFactory::getLeftSideInfo(const QString &_id)
{
    TreeLeftSideInfoJson* json = new TreeLeftSideInfoJson(_id);
    return json;
}
