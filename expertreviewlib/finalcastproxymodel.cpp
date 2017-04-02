#include <QDebug>

#include "finalcastproxymodel.h"
#include "fulltreetablemodel.h"
#include "projectcalculation.h"

#include "serialization/nodesinfo/treerightsidevalues.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"


FinalCastProxyModel::FinalCastProxyModel(const QString &_treeName, const ProjectsLoaderPtr _loader)
    : QIdentityProxyModel(),
      m_treeName(_treeName),
      m_loader(_loader),
      m_source(new FullTreeTableModel(_treeName, _loader)),
      m_average(nullptr)
{
    setSourceModel(m_source);

    connect(this, QAbstractItemModel::modelAboutToBeReset, this, FinalCastProxyModel::updateArMean);
    updateArMean();

    QString leftSidId = m_average->leftSideId();
    TreeLeftSideInfo* lSide = m_loader->getLeftSideInfo(leftSidId);
    m_planeKeys = lSide->planeKeys();
    m_planeKeys.pop_front();
}

FinalCastProxyModel::~FinalCastProxyModel()
{
    qDebug() << Q_FUNC_INFO;
    delete m_source; //TODO fix it
}

QVariant FinalCastProxyModel::data(const QModelIndex &_proxyIndex, int _role) const
{
    if(_role == Qt::DisplayRole)
    {
        int baseColumnCount = QIdentityProxyModel::columnCount();
        if(_proxyIndex.column() == baseColumnCount)
        {
            const QString& key = m_planeKeys.at(_proxyIndex.row());
            return m_average->values()[key];
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

void FinalCastProxyModel::updateArMean()
{
    m_average = ProjectCalculator::getAverageRightSide(m_loader, m_treeName);
}
