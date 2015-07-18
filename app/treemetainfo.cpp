#include <QStack>
#include <QFile>
#include <QTextStream>

#include "treemetainfo.h"
#include "properynode.h"

TreeMetaInfo::TreeMetaInfo()
    : m_openedFile("backup.txt")
{
    open(m_openedFile);
}

TreeMetaInfo::~TreeMetaInfo()
{
    clearNodes();
}

QList<ProperyNode *> TreeMetaInfo::createTestTree()
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

const QList<ProperyNode *> TreeMetaInfo::nodes()
{
    return m_nodes;
}

void TreeMetaInfo::open(const QString &_filename)
{
    m_openedFile = _filename;
    clearNodes();

    QFile file(_filename);
    bool success = file.open(QFile::ReadOnly);
    Q_ASSERT(success);
    if(success)
    {
        QTextStream in(&file);
        QStack<ProperyNode*> nodesHierarhy;
        while(!in.atEnd())
        {
            QString line = in.readLine();

            ProperyNode* node = new ProperyNode();
            node->setDescription(line.trimmed());

            int enclosure = calculateEnclosure(line);

            while(nodesHierarhy.size() > enclosure)
                nodesHierarhy.pop();
            if(enclosure == 0)
                m_nodes << node;
            else
                nodesHierarhy.last()->addChild(node);

            nodesHierarhy.push(node);
        }
    }
}

void TreeMetaInfo::save(const QString &_filename) const
{

}

// 4 spaces or one tab == 1 enclosure level
int TreeMetaInfo::calculateEnclosure(QString &_str)
{
    int i = 0, spaces = 0, tabs = 0;
    int size = _str.size();
    while(i < size)
    {
        if(_str.at(i) == ' ')
            ++spaces;
        else if(_str.at(i) == '\t')
            ++tabs;
        else
            break;
        ++i;
    }
    Q_ASSERT((spaces % 4) == 0);
    return spaces / 4 + tabs;
}

void TreeMetaInfo::clearNodes()
{
    qDeleteAll(m_nodes);
}
