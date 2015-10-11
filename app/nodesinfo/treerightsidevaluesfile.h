#ifndef TREERIGHTSIDEVALUESFILE_H
#define TREERIGHTSIDEVALUESFILE_H

#include "treerightsidevalues.h"

class TreeRightSideValuesFile : public TreeRightSideValues
{
public:
    TreeRightSideValuesFile();
    ~TreeRightSideValuesFile() {}

    QVariantList values() const;
    void setValues(const QVariantList& _values);

    void readValues(const QString &_id);
    void writeValues(const QString& _id);

private:
    QVariantList m_values;
};

#endif // TREERIGHTSIDEVALUESFILE_H
