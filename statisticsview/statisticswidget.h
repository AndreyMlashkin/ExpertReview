#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include "../statisticsview/export.h"

#include <QWidget>
#include <QVector>

namespace Ui
{
class Statistics;
}

class QStandardItemModel;

namespace statistics
{
class Statistic;

class STATISTIC_VIEW_EXPORT StatisticsWidget : public QWidget
{
    Q_OBJECT
public:
    StatisticsWidget(QWidget* _parent = 0);
    ~StatisticsWidget();
    StatisticsWidget& operator <<(Statistic* _stat);

    void addStatistic(Statistic* _stat);
    void clear();

private:
    void updateCombobox();
    void resetModel();

private slots:
    void showOtherStatistics(int _statInd);

protected:
    Ui::Statistics* m_ui;
    QStandardItemModel* m_model;
    QVector<Statistic*> m_stats;
};

}
#endif // STATISTICSWIDGET_H
