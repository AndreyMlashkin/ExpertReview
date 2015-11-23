#ifndef PROJECTCALCULATION_H
#define PROJECTCALCULATION_H

#include <QString>
#include <QMap>

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
    ~ProjectCalculator();

    void calculate(TreeLeftSideInfo* _source, TreeLeftSideInfo* _result);

    void calculate(TreeRightSideValues* _oneProject, TreeRightSideValues* _otherProject,
                   TreeRightSideValues* _result1,    TreeRightSideValues* _result2);

private:
    QMap<QString, double> calculateProject(const QMap<QString, double>& _source);
    static QString substitute(const QString& _expression, const QMap<QString, double>& _source);

private:
    parser::ParserAdaptor* m_adaptor;
    TreeLeftSideInfo* m_methodicJudges;
    TreeRightSideValues* m_metodicJudgesAverage;
    TreeRightSideValues* m_sectionsAverage;
};

#endif // PROJECTCALCULATION_H
