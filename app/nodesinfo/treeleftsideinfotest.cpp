#include <QStringList>

#include "treeleftsideinfotest.h"
#include "properynode.h"

const QList<ProperyNode *> TreeLeftSideInfoTest::nodes()
{
    ProperyNode* economic = new ProperyNode();
    economic->setDescription("Экономические");

    ProperyNode* p1 = new ProperyNode();
    p1->setDescription("Динамика (увеличение) отчислений по налогу на добавленную стоимость (НДС)");

    ProperyNode* p2 = new ProperyNode();
    p2->setDescription("Динамика (увеличение) отчислений по налогу на прибыль");

    economic->addChild(p1);
    economic->addChild(p2);

    ProperyNode* risks = new ProperyNode();
    risks->setDescription("Риски");

    ProperyNode* r1 = new ProperyNode();
    r1->setDescription("Риски техногенного и природного характера");

    risks->addChild(r1);

    QList<ProperyNode *> ans;
    ans << economic << risks;

    return ans;
}

QStringList TreeLeftSideInfoTest::planeNodes()
{
    //!!!
    return QStringList();
}
