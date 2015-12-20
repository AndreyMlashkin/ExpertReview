#ifndef PROJECTAPI_H
#define PROJECTAPI_H

#include <QVariantList>
#include <QLocale>

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

inline double toDouble(const QString &_str)
{
    QString str = _str.simplified();
    if(str.isEmpty())
        return 0;

    bool isOk;
    double ans = QLocale::system().toDouble(str, &isOk);
    if(!isOk)
        ans = str.toDouble(&isOk);
//    Q_ASSERT(isOk);
    if(!isOk)
        return 0;

    return ans;
}

inline double toDouble(const QVariant &_var)
{
    return toDouble(_var.toString());
}

#endif // PROJECTAPI_H
