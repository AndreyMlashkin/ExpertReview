#include "treerightsidevaluesfile.h"

TreeRightSideValuesFile::TreeRightSideValuesFile()
{
}

QVariantList TreeRightSideValuesFile::values() const
{
    return m_values;
}

void TreeRightSideValuesFile::setValues(const QVariantList &_values)
{
    m_values = _values;
}

void TreeRightSideValuesFile::readValues(const QString _id)
{

}

void TreeRightSideValuesFile::writeValues(const QString _id)
{

}
