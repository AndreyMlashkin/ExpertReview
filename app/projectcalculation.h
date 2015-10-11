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

};

#endif // PROJECTCALCULATION_H
