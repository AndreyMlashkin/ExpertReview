#include <QDebug>

#include "serialization/nodesinfo/treeleftsideinfofactory.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"
#include "serialization/nodesinfo/treerightsidevalues.h"

#include "fulltreetablemodel.h"

FullTreeTableModel::FullTreeTableModel(const ProjectsLoaderPtr _loader)
    : QAbstractTableModel(),
      m_loader(_loader)
{}

FullTreeTableModel::FullTreeTableModel(const QString &_treeName, const ProjectsLoaderPtr _loader)
    : QAbstractTableModel(),
      m_loader(_loader),
      m_treeName(_treeName)
{
    setTreeName(_treeName);
}

void FullTreeTableModel::setTreeName(const QString &_treeName)
{
    beginResetModel();

    clear();

    m_treeName = _treeName;
    TreeLeftSideInfo* leftSide = m_loader->getLeftSideInfo(_treeName);
    if(!leftSide)
    {
        qWarning() << Q_FUNC_INFO << " No left side with name " << _treeName;
        clear();
        return;
    }
    Q_ASSERT(_treeName == leftSide->treeName());

    m_columnsNames = leftSide->savedRightSidesTreeNames();
    m_linesNames   = leftSide->planeDescriptions();
    QStringList lineKeys = leftSide->planeKeys();

   // int columnCount = m_columnsNames.count();
    int rowCount    = m_linesNames.count();

    m_values.resize(rowCount);
    int i = 0;
    foreach(QString rightSideName, m_columnsNames)
    {
        TreeRightSideValues* rightSide = m_loader->getOrCreateRightSide(_treeName, rightSideName);
        Q_ASSERT(rightSide);
        ++i;

        QMap<QString, double> values = rightSide->values();

        for(int i = 0; i < m_linesNames.size(); ++i)
        {
            QString lineKey = lineKeys[i];
            double lineValue = values[lineKey];
            m_values[i] << lineValue;
        }
    }
    endResetModel();
}

void FullTreeTableModel::update()
{
    setTreeName(m_treeName);
}

void FullTreeTableModel::clear()
{
//    m_treeName = QString::null;
    m_linesNames = QStringList();
    m_values.resize(0);
}

QModelIndex FullTreeTableModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
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

    if(orientation == Qt::Vertical   && m_linesNames.size() > section)
        return m_linesNames.at(section);
    if(orientation == Qt::Horizontal && m_columnsNames.size() > section)
        return m_columnsNames.at(section);
    return QVariant();
}

void FullTreeTableModel::setBaseName(const QString &_baseName)
{
    int count = m_columnsNames.size();
    for(int i = 0; i < count; ++i)
        m_columnsNames[i] = _baseName + QString::number(i + 1);
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
        Line& line = m_values[i];
        Q_ASSERT(line.size() == 2);

        normaliseLine(line);
    }
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

void FullTreeTableModel::normaliseLine(FullTreeTableModel::Line &_line)
{
    double maxVal = -1;
    foreach(double val, _line)
        maxVal = qMax(maxVal, val);

    if(maxVal == 0)
        return;

    for(double& val : _line)
        val /= maxVal;
}
