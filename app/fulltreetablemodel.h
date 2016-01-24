#ifndef FULLTREETABLEMODEL_H
#define FULLTREETABLEMODEL_H

#include <QAbstractTableModel>


// TODO find better name
class FullTreeTableModel : public QAbstractTableModel
{
public:
    FullTreeTableModel();
    FullTreeTableModel(const QString& _treeName);

    void setTreeName(const QString& _treeName);
    void update();
    void clear();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void setBaseName(const QString& _baseName);
    void normaliseRows();

    QString report() const;

private:
    typedef QVector< double> Line;
    void normaliseLine(Line& _line);

private:
    QString m_treeName;

    QStringList m_linesNames;
    QStringList m_columnsNames;

    QVector<Line> m_values;
};

#endif // FULLTREETABLEMODEL_H
