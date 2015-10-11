#ifndef TREERIGHTSIDEVALUESFILE_H
#define TREERIGHTSIDEVALUESFILE_H

#include "treerightsidevalues.h"

class TreeRightSideValuesFile : public TreeRightSideValues
{
public:
    TreeRightSideValuesFile();
    ~TreeRightSideValuesFile() {}

    QVariantList values() const override;
    void setValues(const QVariantList& _values) override;

    void readValues(const QString &_id) override;
    void writeValues(const QString& _id) override;

private:
    QVariantList m_values;
};

#endif // TREERIGHTSIDEVALUESFILE_H
