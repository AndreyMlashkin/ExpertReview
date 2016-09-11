#include <QStandardItemModel>

#include <statisticswidget.h>
#include <statistic.h>
#include "ui_statistic.h"

namespace statistics
{

QColor get_color_for_chart(int _pos)
{
    static QVector<QColor> colors;
    if(colors.isEmpty())
    {
        colors  << QString("#99e600")
                << QString("#99cc00")
                << QString("#99b300")
                << QString("#9f991a")
                << QString("#a48033")
                << QString("#a9664d")
                << QString("#ae4d66")
                << QString("#b33380")
                << QString("#a64086")
                << QString("#994d8d")
                << QString("#8d5a93")
                << QString("#806699")
                << QString("#8073a6")
                << QString("#8080b3");
    }
    return colors.at(_pos % colors.size());
}

StatisticsWidget::StatisticsWidget(QWidget *_parent)
    : QWidget(_parent),
      m_ui(new Ui::Statistics),
      m_model(new QStandardItemModel)
{
    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    m_ui->diagramm->setModel(m_model);
//    setupCombobox();
//    showOtherStatistics(0);
    connect(m_ui->statisticType, SIGNAL(currentIndexChanged(int)), this, SLOT(showOtherStatistics(int)));
}

StatisticsWidget::~StatisticsWidget()
{
    delete m_ui;
    delete m_model;
}

StatisticsWidget &StatisticsWidget::operator <<(Statistic *_stat)
{
    addStatistic(_stat);
    return *this;
}

void StatisticsWidget::addStatistic(Statistic *_stat)
{
    m_stats << _stat;
    m_ui->statisticType->addItem(_stat->name());
    showOtherStatistics(0);
}

void StatisticsWidget::clear()
{
    m_stats.clear();
    updateCombobox();
    showOtherStatistics(0);
}

void StatisticsWidget::updateCombobox()
{
    m_ui->statisticType->clear();
    foreach(Statistic* s, m_stats)
        m_ui->statisticType->addItem(s->name());
}

void StatisticsWidget::resetModel()
{
    m_model->removeColumn(0);
    m_model->removeColumn(1);
    m_model->setColumnCount(2);
}

void StatisticsWidget::showOtherStatistics(int _statInd)
{
    resetModel();

    if(_statInd >= m_stats.size())
        return;

    Statistic* statistics = m_stats.at(_statInd);
    int size = statistics->values().size();
    m_model->setRowCount(size);

    for(int i = 0; i < size; ++i)
    {
        m_model->insertRows(i, 1, QModelIndex());

        m_model->setData(m_model->index(i, 0, QModelIndex()), statistics->description().at(i));
        m_model->setData(m_model->index(i, 1, QModelIndex()), statistics->values().at(i));
        m_model->setData(m_model->index(i, 0, QModelIndex()), get_color_for_chart(i), Qt::DecorationRole);
    }
    m_ui->diagramm->dataChanged(QModelIndex(), QModelIndex());

    //delete statistics;
    return;
}

}
