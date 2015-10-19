#ifndef PROJECTCALCULATION_H
#define PROJECTCALCULATION_H

#include <QString>

class TreeRightSideValues;
class TreeLeftSideInfo;

class ProjectCalculator
{
public:
    ProjectCalculator(TreeRightSideValues* _metodicJudgesAverage, TreeRightSideValues* _sectionsAverage);

    void calculate(TreeLeftSideInfo* _source, TreeLeftSideInfo* _result);

    void calculate(TreeRightSideValues* _oneProject, TreeRightSideValues* _otherProject,
                   TreeRightSideValues* _result1,    TreeRightSideValues* _result2);

private:
    static QList<double> multiplyByGroups(const QList<double>& _vals, const QList<double>& _groupsKoeff);
    static QList<int> groupTitles();

private:
    TreeRightSideValues* m_metodicJudgesAverage;
    TreeRightSideValues* m_sectionsAverage;
};

#endif // PROJECTCALCULATION_H
