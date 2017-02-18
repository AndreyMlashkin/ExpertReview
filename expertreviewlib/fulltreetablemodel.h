#ifndef FULLTREETABLEMODEL_H
#define FULLTREETABLEMODEL_H

#include <QAbstractTableModel>
#include "serialization/projectsloader.h"

//!
//! \brief The FullTreeTableModel class класс для просмотра всех векторов значений одного проекта
//! в табличном виде
//!
class FullTreeTableModel : public QAbstractTableModel
{
public:
    FullTreeTableModel(const ProjectsLoaderPtr _loader);
    FullTreeTableModel(const QString& _treeName, const ProjectsLoaderPtr _loader);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex &ind = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void setBaseName(const QString& _baseName);
    void normaliseRows();

    QString report() const;

public slots:
    void setTreeName(const QString& _treeName);
    void update();
    void clear();

private:
    typedef QVector< double> Line;
    void normaliseLine(Line& _line);

private:
    ProjectsLoaderPtr m_loader;
    QString m_treeName;

    QStringList m_linesNames;
    QStringList m_columnsNames;

    QVector<Line> m_values;
};

#endif // FULLTREETABLEMODEL_H
