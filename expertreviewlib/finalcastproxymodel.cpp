#include "finalcastproxymodel.h"
#include "fulltreetablemodel.h"

FinalCastProxyModel::FinalCastProxyModel(const QString &_treeName, const ProjectsLoaderPtr _loader)
    : QIdentityProxyModel(),
      m_loader(_loader),
      m_source(new FullTreeTableModel(_treeName, _loader))
{
    setSourceModel(m_source);
}

FinalCastProxyModel::~FinalCastProxyModel()
{
    delete m_source;
}

QVariant FinalCastProxyModel::data(const QModelIndex &_proxyIndex, int _role) const
{
    if(_role == Qt::DisplayRole)
    {
        int baseColumnCount = QIdentityProxyModel::columnCount();
        if(_proxyIndex.column() == baseColumnCount)
        {
            return QVariant();
        }
        if(_proxyIndex.column() == baseColumnCount + 1)
        {
            return QVariant();
        }
    }
    return QIdentityProxyModel::data(_proxyIndex, _role);
}

QVariant FinalCastProxyModel::headerData(int _section, Qt::Orientation _orientation, int _role) const
{
    if(_role == Qt::DisplayRole &&
       _orientation == Qt::Horizontal)
    {
        int baseColumnCount = QIdentityProxyModel::columnCount();
        if(_section == baseColumnCount)
            return "Среднее арифметическое";
        if(_section == baseColumnCount + 1)
            return "ЛПР";
    }
    return QIdentityProxyModel::headerData(_section, _orientation, _role);
}

int FinalCastProxyModel::columnCount(const QModelIndex &parent) const
{
    return m_source->columnCount(parent) + 2;
}

void FinalCastProxyModel::setSourceModel(FullTreeTableModel *sourceModel)
{
    QIdentityProxyModel::setSourceModel(sourceModel);
}
