#include <QStack>
#include <QFile>
#include <QTextStream>

#include "treeleftsideinfofile.h"
#include "properynode.h"

TreeLeftSideInfoFile::TreeLeftSideInfoFile()
    : TreeLeftSideInfo()
{}

TreeLeftSideInfoFile::~TreeLeftSideInfoFile()
{
    clearNodes();
}

QString TreeLeftSideInfoFile::id() const
{
    return m_openedFile;
}

const QList<ProperyNode *> TreeLeftSideInfoFile::nodes()
{
    return m_nodes;
}

QStringList TreeLeftSideInfoFile::planeNodes()
{
    return m_planeNodes;
}

void TreeLeftSideInfoFile::open(const QString &_treeName)
{
    m_openedFile = _treeName;
    clearNodes();

    QFile file(_treeName);
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
            m_planeNodes << line.trimmed();

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

// 4 spaces or one tab == 1 enclosure level
int TreeLeftSideInfoFile::calculateEnclosure(QString &_str)
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

void TreeLeftSideInfoFile::clearNodes()
{
    qDeleteAll(m_nodes);
}
