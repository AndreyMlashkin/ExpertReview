#ifndef PROJECTCALCULATION_H
#define PROJECTCALCULATION_H

#include <QString>

class ProjectCalculator
{
public:
    static void calculate(const QString& _metodicJudgesAverage,
                          const QString& _sectionsAverage,
                          const QString& _oneProject, const QString& _otherProject,
                          const QString& _baseResultName);

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
