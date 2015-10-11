#ifndef TREERIGHTSIDEVALUES_H
#define TREERIGHTSIDEVALUES_H

#include <QStringList>
#include <QVariant>

class TreeRightSideValues
{
public:
    TreeRightSideValues() {}
    virtual ~TreeRightSideValues() {}

    virtual QVariantList values() const = 0;
    virtual void setValues(const QVariantList& _values) = 0;

    virtual void readValues(const QString _id) = 0;
    virtual void writeValues(const QString _id) = 0;
};

#endif // TREERIGHTSIDEVALUES_H
