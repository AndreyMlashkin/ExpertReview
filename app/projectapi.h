#ifndef PROJECTAPI_H
#define PROJECTAPI_H

#include <QVariantList>

inline QList<double> toDoubleList(const QVariantList& _varList)
{
    QList<double> ans;
    foreach(QVariant v, _varList)
        ans << v.toDouble();
    return ans;
}

inline QVariantList toVariantList(const QList<double>& _doubleList)
{
    QVariantList ans;
    foreach(double d, _doubleList)
        ans << QVariant(d);
    return ans;
}

#endif // PROJECTAPI_H
