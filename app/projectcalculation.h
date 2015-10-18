#ifndef PROJECTCALCULATION_H
#define PROJECTCALCULATION_H

#include <QString>

class TreeRightSideValues;
class TreeInfoFactory;

class ProjectCalculator
{
public:
    ProjectCalculator(const QString& _metodicJudgesAverage, const QString& _sectionsAverage, TreeInfoFactory* _factory);
    ProjectCalculator(TreeRightSideValues* _metodicJudgesAverage, TreeRightSideValues* _sectionsAverage, TreeInfoFactory* _factory);

    void calculate(const QString& _oneProject, const QString& _otherProject,
                   const QString& _result1,    const QString& _result2);

    void calculate(TreeRightSideValues* _oneProject, TreeRightSideValues* _otherProject,
                   TreeRightSideValues* _result1,    TreeRightSideValues* _result2);

private:
    TreeInfoFactory* m_factory;
    TreeRightSideValues* m_metodicJudgesAverage;
    TreeRightSideValues* m_sectionsAverage;


//    static void calculate(const QString& _metodicJudgesAverage,
//                          const QString& _sectionsAverage,
//                          const QString& _oneProject, const QString& _otherProject,
//                          const QString& _baseResultName);

//    static void calculate(const QString& _metodicJudgesAverage,
//                          const QString& _sectionsAverage,
//                          const QString& _project,
//                          const QString& _resultName);

private:
    static QList<double> multiply(const QList<double>& _values, const QList<double>& _average);
    static QList<double> multiplyByGroups(const QList<double>& _vals, const QList<double>& _groupsKoeff);

    static QList<int> groupTitles();

};

#endif // PROJECTCALCULATION_H
