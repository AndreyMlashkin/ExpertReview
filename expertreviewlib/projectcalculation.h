#ifndef PROJECTCALCULATION_H
#define PROJECTCALCULATION_H

#include <QString>
#include <QMap>
#include "serialization/projectsloader.h"

class TreeRightSideValues;
class TreeLeftSideInfo;

namespace parser
{
    class ParserAdaptor;
}

//!
//! \brief The ProjectCalculator class класс, осуществляющий расчёт проектов.
//!
class ProjectCalculator
{
public:
    ProjectCalculator(TreeLeftSideInfo* _methodicJudges, TreeRightSideValues* _metodicJudgesAverage, TreeRightSideValues* _sectionsAverage);
    ~ProjectCalculator();

    //! \brief sumAll возвращает сумму всех значений вектора
    static double sumAll(const QList<double> &_values);

    //!
    //! \brief getAverageRightSide получить среднее значение проекта
    //! \param _leftSide имя проекта
    static TreeRightSideValues* getAverageRightSide(ProjectsLoaderPtr& _loader,
                                           const QString& _leftSide);
    //!
    //! \brief normalise нормализует вектор значений одного проекта
    static TreeRightSideValues* normalise(ProjectsLoaderPtr &_loader, TreeRightSideValues *_values);

    //!
    //! \brief calculate произвести расчёт одного проекта и записать в _result
    //! \param _source проект, который будет расчитан
    //! \param _result возвращаемое значение
    void calculate(TreeLeftSideInfo* _source, TreeLeftSideInfo* _result, const QString &_formulsPath);

    //!
    //! \brief calculate произвести расчёт обоих проектов
    //! \param _oneProject вектор значений одного проекта
    //! \param _otherProject вектор значений другого проекта
    //! \param _result1 вектор значений результата первого проекта
    //! \param _result2 вектор значений результата второго проекта
    void calculate(TreeRightSideValues* _oneProject, TreeRightSideValues* _otherProject,
                   TreeRightSideValues* _result1,    TreeRightSideValues* _result2,
                   const QString& _formulsPath);

    //!
    //! \brief updateSectionCalculation производит перерасчёт секций и записывает в _loader
    static void updateSectionCalculation(ProjectsLoaderPtr& _loader);

    //!
    //! \brief normalise нормализовать два вектора значений проектов относительно друг друга,
    //! т.е. в одной строке должно быть 1, в другой приведённое значение.
    static void normalise(double& _one, double& _other);// !!! make universal

private:
    QMap<QString, double> calculateProject(const QMap<QString, double>& _source,
                                           const QString& _formulsFilename);
    static QString substitute(const QString& _expression, const QMap<QString, double>& _source);

private:
    parser::ParserAdaptor* m_adaptor;
    TreeLeftSideInfo* m_methodicJudges;
    TreeRightSideValues* m_metodicJudgesAverage;
    TreeRightSideValues* m_sectionsAverage;
};

#endif // PROJECTCALCULATION_H
