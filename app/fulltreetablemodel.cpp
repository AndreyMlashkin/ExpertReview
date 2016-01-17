#include <QDebug>
#include "nodesinfo/treeleftsideinfofactory.h"
#include "nodesinfo/treeleftsideinfo.h"
#include "nodesinfo/treerightsidevalues.h"

#include "fulltreetablemodel.h"
#include "projectcalculation.h" // !!! remove dependency

FullTreeTableModel::FullTreeTableModel()
    : QAbstractTableModel()
{
}

FullTreeTableModel::FullTreeTableModel(const QString &_treeName)
    : QAbstractTableModel(),
    m_treeName(_treeName)
{
    setTreeName(_treeName);
}

void FullTreeTableModel::setTreeName(const QString &_treeName)
{
    beginResetModel();

    clear();

    m_treeName = _treeName;
    TreeLeftSideInfoFactory* factory = new TreeLeftSideInfoFactory();
    TreeLeftSideInfo* leftSide = factory->getLeftSideInfo(_treeName);

    m_columnsNames = leftSide->savedRightSidesTreeNames();
    m_linesNames   = leftSide->planeDescriptions();
    QStringList lineKeys = leftSide->planeKeys();

    int columnCount = m_columnsNames.count();
    int rowCount    = m_linesNames.count();

    m_values.resize(rowCount);
    foreach(QString rightSideName, m_columnsNames)
    {
        TreeRightSideValues* rightSide = leftSide->createRightSide();
        rightSide->readValues(rightSideName);

        QMap<QString, double> values = rightSide->values();

        for(int i = 0; i < m_linesNames.size(); ++i)
        {
            QString lineKey = lineKeys[i];
            double lineValue = values[lineKey];
            m_values[i] << lineValue;
        }
    }

/*
    foreach (const QString name, m_columnsNames)
    {
        TreeRightSideValues* values = leftSide->createRightSide();
        values->readValues(name);
        values->values();
    }
*/
    delete leftSide;
    delete factory;

    endResetModel();
}

void FullTreeTableModel::update()
{
    setTreeName(m_treeName);
}

void FullTreeTableModel::clear()
{
    m_treeName = QString::null;
    m_linesNames = QStringList();
    m_values.resize(0);
}

QModelIndex FullTreeTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

int FullTreeTableModel::rowCount(const QModelIndex &) const
{
    return m_linesNames.size();
}

int FullTreeTableModel::columnCount(const QModelIndex &) const
{
    return m_columnsNames.size();
}

QVariant FullTreeTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole && index.isValid())
        return m_values[index.row()][index.column()];

    return QVariant();
}

QVariant FullTreeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Vertical)
        return m_linesNames.at(section);
    if(orientation == Qt::Horizontal)
        return m_columnsNames.at(section);
    return QVariant();
}

void FullTreeTableModel::setBaseName(const QString &_baseName)
{
    int count = m_columnsNames.size();
    for(int i = 0; i < count; ++i)
        m_columnsNames[i] = _baseName + QString::number(i);
}

inline double sum(const QVector<double> vals)
{
    double res = 0;
    foreach(const double v, vals)
        res += v;
    return res;
}

void FullTreeTableModel::normaliseRows()
{
    beginResetModel();

    for(int i = 0; i < m_values.size(); ++i)
    {
        auto line = m_values[i];
        Q_ASSERT(line.size() == 2);

        ProjectCalculator::normalise(line[0], line[1]);

        m_values[i] = line;
    }


/*    for(int i = 0; i < m_values.size(); ++i)
    {
        auto line = m_values[i];
        double lineSum = sum(line);
        for(int j = 0; j < line.size(); ++j)
            line[j] /= lineSum;

        m_values[i] = line;
    }
*/
    endResetModel();
}

QString FullTreeTableModel::report() const
{
    QString ans;

    foreach(QString line, m_columnsNames)
        ans += line + "\t";
    ans += "\r\n";

    for(int i = 0; i < m_linesNames.size(); ++i)
    {
        ans += m_linesNames.at(i) + "\t";
        foreach (double val, m_values.at(i))
        {
            ans += QString::number(val) + "\t";
        }
        ans += "\r\n";
    }
    return ans;
}
