#ifndef RIGHTSIDEDIAGRAMMVIEW_H
#define RIGHTSIDEDIAGRAMMVIEW_H

#include "statisticswidget.h"

class TreeLeftSideInfo;

//!
//! \brief The RightSideDiagrammView class класс графического интерфейса,
//! отображающий круговую диаграмму для вектора значений одного проекта.
//!
class RightSideDiagrammView : public statistics::StatisticsWidget
{
    Q_OBJECT
public:
    explicit RightSideDiagrammView(TreeLeftSideInfo* _leftSide, QWidget *parent = 0);
    void updateStatistics(TreeLeftSideInfo* _leftSide);

signals:

public slots:

private:
    TreeLeftSideInfo* m_leftSide;
};

#endif // RIGHTSIDEDIAGRAMMVIEW_H
