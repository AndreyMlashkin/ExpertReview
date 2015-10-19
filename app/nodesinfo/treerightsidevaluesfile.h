#ifndef TREERIGHTSIDEVALUESFILE_H
#define TREERIGHTSIDEVALUESFILE_H

#include "treerightsidevalues.h"

class TreeRightSideValuesFile : public TreeRightSideValues
{
public:
    TreeRightSideValuesFile(const QStringList& _orderedKeys);
    ~TreeRightSideValuesFile() {}

    QMap<QString, double> values() const override;
    void setValues(const QMap<QString, double>& _values) override;

    void readValues(const QString &_id) override;
    void writeValues(const QString& _id) override;

private:
    void fillWithEmptyVals();

private:
    QStringList m_orderedKeys;
    QMap<QString, double> m_values;
};

#endif // TREERIGHTSIDEVALUESFILE_H
