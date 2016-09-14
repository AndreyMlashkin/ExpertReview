#include "simplystatistic.h"

namespace statistics
{

SimplyStatistic::SimplyStatistic()
    : statistics::Statistic()
{}

SimplyStatistic::~SimplyStatistic()
{

}

QString SimplyStatistic::name() const
{
    return m_name;
}

const QVector<int> SimplyStatistic::values() const
{
    return m_values;
}

const QStringList SimplyStatistic::description() const
{
    return m_description;
}

void SimplyStatistic::setName(const QString &_name)
{
    m_name = _name;
}

void SimplyStatistic::setValues(const QVector<int> &_values)
{
    m_values = _values;
}

void SimplyStatistic::setDescription(const QStringList &_description)
{
    m_description = _description;
}

}
