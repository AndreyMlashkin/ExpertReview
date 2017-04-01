#ifndef PROJECTAPI_H
#define PROJECTAPI_H

#include "export.h"

#include <QDebug>
#include <QVariantList>
#include <QLocale>

#define declareConstant(constantName) static const QString constantName (QStringLiteral(#constantName))

namespace fileNames
{
    static const QString log        (QStringLiteral("log.txt"));
    static const QString report     (QStringLiteral("report.csv"));
    static const QString patternsDir(QStringLiteral("project_patterns"));
}

inline QStringList toStringList(const QVariantList& _varList)
{
    QStringList result;
    for(QVariant v : _varList)
        result << v.toString();

    return result;
}

inline QList<double> toDoubleList(const QVariantList& _varList)
{
    QList<double> ans;
    for(const QVariant& v : _varList)
        ans << v.toDouble();
    return ans;
}

inline QList<int> toIntList(const QStringList& _strList)
{
    QList<int> ans;
    for(QString v : _strList)
        ans << v.toInt();
    return ans;
}

inline QVariantList toVariantList(const QList<double>& _doubleList)
{
    QVariantList ans;
    for(double d : _doubleList)
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
