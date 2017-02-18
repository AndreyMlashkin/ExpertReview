#ifndef SIMPLYSTATISTIC_H
#define SIMPLYSTATISTIC_H

#include <QVector>
#include <QStringList>

#include "statistic.h"

namespace statistics
{

//!
//! \brief The SimplyStatistic class класс графического интерфейса, позволяющий просматривать
//! круговые диаграммы
//!
class SimplyStatistic : public statistics::Statistic
{
public:
  //  SimplyStatistic();
    SimplyStatistic();
    ~SimplyStatistic();

    QString name() const override;
    const QVector<int> values() const override;
    const QStringList description() const override;

    void setName(const QString& _name);
    void setValues(const QVector<int>& _values);
    void setDescription(const QStringList& _description);

private:
    QString m_name;
    QVector<int> m_values;
    QStringList m_description;
};

}
#endif // SIMPLYSTATISTIC_H
