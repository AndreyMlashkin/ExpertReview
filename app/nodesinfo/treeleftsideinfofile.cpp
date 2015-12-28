#include <QDebug>
#include <QStack>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "treeleftsideinfofile.h"
#include "treerightsidevaluesfile.h"
#include "propertynode.h"

TreeLeftSideInfoFile::TreeLeftSideInfoFile(const QString &_treeName)
    : TreeLeftSideInfo()
{
    open(_treeName);
}

TreeLeftSideInfoFile::~TreeLeftSideInfoFile()
{
    clearNodes();
}

QString TreeLeftSideInfoFile::treeName() const
{
    return m_openedFile;
}

const QList<PropertyNode *> TreeLeftSideInfoFile::nodes()
{
    return m_nodes;
}

QStringList TreeLeftSideInfoFile::planeDescriptions() const
{
    return m_planeDescriptions;
}

QStringList TreeLeftSideInfoFile::planeKeys() const
{
    return m_planeKeys;
}

int TreeLeftSideInfoFile::savedRightSidesCount() const
{
    int count = 0;
    forever
    {
        QString path = m_openedFile + QString::number(count);
        QFileInfo info(path);
        if(info.exists())
            ++count;
        else
            return count;
    }
}

QStringList TreeLeftSideInfoFile::savedRightSidesIds() const
{
    QStringList ans;
    int count = 0;
    forever
    {
        QString path = m_openedFile + QString::number(count);
        QFileInfo info(path);
        if(info.exists())
        {
            ++count;
            ans << path;
        }
        else
            return ans;
    }
}

QString TreeLeftSideInfoFile::savedAverageRightSideTreeName() const
{
    QString path = m_openedFile + "_average";
    QFileInfo info(path);
    if(info.exists())
        return path;
    return QString();
}

TreeRightSideValues *TreeLeftSideInfoFile::createRightSide() const
{
    TreeRightSideValues* values = new TreeRightSideValuesFile(m_planeKeys);
    return values;
}

inline QStringList split(const QString& _str)
{
    QStringList splitted = _str.split(QRegExp("[\\s\\t]+#"), QString::SkipEmptyParts);
    for(int i = 0; i < splitted.size(); ++i)
        splitted[i] = splitted[i].trimmed();
    return splitted;
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
        QStack<PropertyNode*> nodesHierarhy;
        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList descriptionAndKey = split(line);

            Q_ASSERT_X(descriptionAndKey.size() == 2, "error in", line.toStdString().c_str());
            if(m_planeKeys.contains(descriptionAndKey.at(1)))
            {
                qDebug() << "Warning! duplicate tag" << descriptionAndKey;
                continue;
            }
            PropertyNode* node = new PropertyNode(descriptionAndKey.at(0),
                                                descriptionAndKey.at(1));

            m_planeDescriptions << descriptionAndKey.at(0);
            m_planeKeys  << descriptionAndKey.at(1);

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
