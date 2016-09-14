#ifndef RIGHTSIDEDIAGRAMMVIEW_H
#define RIGHTSIDEDIAGRAMMVIEW_H

#include "statisticswidget.h"

class TreeLeftSideInfo;

//TODO rename!
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
