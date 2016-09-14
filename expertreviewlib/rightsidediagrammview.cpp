#include <QDebug>
#include "rightsidediagrammview.h"
#include "simplystatistic.h"

#include "serialization/nodesinfo/treeleftsideinfo.h"
#include "serialization/nodesinfo/treerightsidevalues.h"

RightSideDiagrammView::RightSideDiagrammView(TreeLeftSideInfo *_leftSide, QWidget *parent)
    : statistics::StatisticsWidget(parent),
      m_leftSide(_leftSide)
{
    updateStatistics(_leftSide);
}

void RightSideDiagrammView::updateStatistics(TreeLeftSideInfo *_leftSide)
{
    m_leftSide = _leftSide;

    auto rSides = _leftSide->getRightSides();
    QStringList planeDescriptions = _leftSide->planeDescriptions();
    QStringList planeKeys = _leftSide->planeKeys();

    for(TreeRightSideValues* rSide : rSides)
    {
        QString name = rSide->guiName();
        QVector<int> values;
        //QStringList description;

        for(const QString& key : planeKeys)
        {
            values << rSide->values()[key];
        }


//        auto rSideValues = rSide->values();
//        auto iter = rSideValues.begin();
//        while(iter != rSideValues.end())
//        {
//            description << iter.key();
//            values << iter.value();
//            ++iter;
//        }
        statistics::SimplyStatistic* statistic = new statistics::SimplyStatistic();
        statistic->setName(name);
        statistic->setDescription(planeDescriptions);
        statistic->setValues(values);

        qDebug() << Q_FUNC_INFO << m_stats.size();
        addStatistic(statistic);
    }
    qDebug() << Q_FUNC_INFO << m_stats.size();
}
