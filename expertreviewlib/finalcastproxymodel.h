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

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void setSourceModel(FullTreeTableModel* sourceModel);

private slots:
    void updateArMean();

private:
    QString m_treeName;
    ProjectsLoaderPtr m_loader;
    FullTreeTableModel* m_source;
    QStringList m_planeKeys;

    TreeRightSideValues* m_average;
};

#endif // FINALCASTPROXYMODEL_H
