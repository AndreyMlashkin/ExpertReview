#ifndef STATISTIC_H
#define STATISTIC_H

#include "../statisticsview/export.h"
#include <QString>

namespace statistics
{

class STATISTIC_VIEW_EXPORT Statistic
{
public:
    Statistic();
    virtual ~Statistic();
    virtual QString name() const = 0;
    virtual const QVector<int> values() const = 0;
    virtual const QStringList description() const = 0;
};

}
#endif // STATISTIC_H
