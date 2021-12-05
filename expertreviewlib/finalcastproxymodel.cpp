#include <QDebug>
#include <QFont>

#include "projectapi.h"
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
      m_average(nullptr),
      m_finalCast(nullptr)
{
    setSourceModel(m_source);

    connect(this, &QAbstractItemModel::modelAboutToBeReset, this, &FinalCastProxyModel::updateArMean);
    updateArMean();
    updateFinalCast();

    QString leftSidId = m_average->leftSideId();
    Q_ASSERT(leftSidId == _treeName);
    TreeLeftSideInfo* lSide = m_loader->getLeftSideInfo(leftSidId);
    m_planeKeys = lSide->planeKeys();
    m_planeKeys.pop_front();
}

FinalCastProxyModel::~FinalCastProxyModel()
{
    delete m_source;
}

QVariant FinalCastProxyModel::data(const QModelIndex &_proxyIndex, int _role) const
{
    int baseColumnCount = QIdentityProxyModel::columnCount();
    if(_role == Qt::DisplayRole)
    {
        if(_proxyIndex.column() == baseColumnCount)
        {
            const QString& key = m_planeKeys.at(_proxyIndex.row());
            return m_average->values()[key];
        }
    }
    if(_role == Qt::DisplayRole ||
       _role == Qt::EditRole)
    {
        if(_proxyIndex.column() == baseColumnCount + 1)
        {
            const QString& key = m_planeKeys.at(_proxyIndex.row());
            return m_finalCast->values()[key];
        }
    }

    if(_role == Qt::FontRole)
    {
        QVariant variantFont = QIdentityProxyModel::data(_proxyIndex, _role);
        QFont styledFont = qvariant_cast<QFont>(variantFont);
        styledFont.setBold(true);
        //styledFont.setPointSize(m_fontSize);
        return styledFont;
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
            return "Среднее";
        if(_section == baseColumnCount + 1)
            return "ЛПР";
    }

    if(_role == Qt::FontRole)
    {
        QVariant variantFont = QIdentityProxyModel::headerData(_section, _orientation, _role);
        QFont styledFont = qvariant_cast<QFont>(variantFont);
        styledFont.setBold(true);
        //styledFont.setPointSize(m_fontSize);
        return styledFont;
    }

    return QIdentityProxyModel::headerData(_section, _orientation, _role);
}

bool FinalCastProxyModel::setData(const QModelIndex &_index, const QVariant &_value, int _role)
{
    if(_role == Qt::EditRole)
    {
        int baseColumnCount = QIdentityProxyModel::columnCount();
        if(_index.column() == baseColumnCount + 1)
        {
            const QString& key = m_planeKeys.at(_index.row());
            auto values = m_finalCast->values();
            values[key] =  toDouble(_value);

            m_finalCast->setValues(values);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags FinalCastProxyModel::flags(const QModelIndex &_index) const
{
    Qt::ItemFlags flags = QIdentityProxyModel::flags(_index);

    int baseColumnCount = QIdentityProxyModel::columnCount();
    if(_index.column() == baseColumnCount + 1);
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
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

void FinalCastProxyModel::updateFinalCast()
{
    m_finalCast = ProjectCalculator::getFinalCastRightSide(m_loader, m_treeName);
    m_finalCast->setGuiName("ЛПР");
}
