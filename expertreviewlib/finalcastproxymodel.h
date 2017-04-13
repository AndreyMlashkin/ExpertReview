#ifndef FINALCASTPROXYMODEL_H
#define FINALCASTPROXYMODEL_H

#include <QIdentityProxyModel>
#include "serialization/projectsloader.h"

class FullTreeTableModel;

class FinalCastProxyModel : public QIdentityProxyModel
{
public:
    FinalCastProxyModel(const QString& _treeName, const ProjectsLoaderPtr _loader);
    ~FinalCastProxyModel();

public:
    QVariant data(const QModelIndex &_proxyIndex, int _role = Qt::DisplayRole) const override;
    QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &_index, const QVariant &_value, int _role) override;

    Qt::ItemFlags flags(const QModelIndex &_index) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void setSourceModel(FullTreeTableModel* sourceModel);

private slots:
    void updateArMean();
    void updateFinalCast();

private:
    QString m_treeName;
    ProjectsLoaderPtr m_loader;
    FullTreeTableModel* m_source;
    QStringList m_planeKeys;

    TreeRightSideValues* m_average;
    TreeRightSideValues* m_finalCast;
};

#endif // FINALCASTPROXYMODEL_H
