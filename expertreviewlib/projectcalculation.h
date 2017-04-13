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
    ProjectCalculator(const ProjectsLoaderPtr& _loader);
    ~ProjectCalculator();

    //! \brief sumAll возвращает сумму всех значений вектора
    static double sumAll(const QList<double> &_values);

    //!
    //! \brief getAverageRightSide получить среднее значение проекта
    //! \param _leftSide имя проекта
    static TreeRightSideValues* getAverageRightSide(ProjectsLoaderPtr& _loader,
                                           const QString& _leftSide);

    //!
    //! \brief getFinalCastRightSide получить значение ЛПР проекта.
    //! Нулевые значения Final cast будут обновлены средними значениями
    //! \param _leftSide имя проекта
    static TreeRightSideValues* getFinalCastRightSide(ProjectsLoaderPtr& _loader,
                                           const QString& _leftSide);
    //!
    //! \brief normalise нормализует вектор значений одного проекта
    static TreeRightSideValues* normalise(ProjectsLoaderPtr &_loader, TreeRightSideValues *_values);

    void calculate();

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
    ProjectsLoaderPtr m_loader;

    TreeLeftSideInfo* m_methodicJudges;
    TreeRightSideValues* m_metodicJudgesAverage;
    TreeRightSideValues* m_sectionsFinalCast;

    TreeLeftSideInfo* m_constants;
    TreeLeftSideInfo* m_result;
};

#endif // PROJECTCALCULATION_H
