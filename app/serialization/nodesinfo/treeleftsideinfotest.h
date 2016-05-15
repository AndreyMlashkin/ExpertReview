#ifndef TREELEFTSIDEINFOTEST_H
#define TREELEFTSIDEINFOTEST_H

#include "treeleftsideinfo.h"

class TreeLeftSideInfoTest : public TreeLeftSideInfo
{
public:
    TreeLeftSideInfoTest() {}
    ~TreeLeftSideInfoTest() {}

    const QList<ProperyNode*> nodes() override;
    QStringList planeNodes() override;

    void open(const QString&) override {}

private:
    QStringList m_planeNodes;
};

#endif // TREELEFTSIDEINFOTEST_H
