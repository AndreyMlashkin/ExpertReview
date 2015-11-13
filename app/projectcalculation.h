#ifndef PROJECTCALCULATION_H
#define PROJECTCALCULATION_H

#include <QString>

class TreeRightSideValues;
class TreeLeftSideInfo;
namespace parser
{
    class ParserAdaptor;
}

// should be refactored
class ProjectCalculator
{
public:
    ProjectCalculator(TreeLeftSideInfo* _methodicJudges, TreeRightSideValues* _metodicJudgesAverage, TreeRightSideValues* _sectionsAverage);

    void calculate(TreeLeftSideInfo* _source, TreeLeftSideInfo* _result);

    void calculate(TreeRightSideValues* _oneProject, TreeRightSideValues* _otherProject,
                   TreeRightSideValues* _result1,    TreeRightSideValues* _result2);

private:
    parser::ParserAdaptor* m_adaptor;
    TreeLeftSideInfo* m_methodicJudges;
    TreeRightSideValues* m_metodicJudgesAverage;
    TreeRightSideValues* m_sectionsAverage;
};

#endif // PROJECTCALCULATION_H
