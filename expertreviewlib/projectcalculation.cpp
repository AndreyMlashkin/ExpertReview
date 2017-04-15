#include <QDebug>
#include <QStringList>
#include <QFile>

#include <parseradaptor.h>
#include "projectapi.h"
#include "projectcalculation.h"
#include "serialization/nodesinfo/leftsidesconstants.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"
#include "serialization/nodesinfo/treerightsidevalues.h"
#include "serialization/nodesinfo/propertynode.h"

QMap<QString, double> multiply(const QMap<QString, double>& _one, const QMap<QString, double>& _other);
QMap<QString, double> multiplyWithSection(const QMap<QString, double>& _values, const QMap<QString, double>& _sections, const QList<PropertyNode*>& _nodes);

void removeNan(QMap<QString, double>& _values);

ProjectCalculator::ProjectCalculator(const ProjectsLoaderPtr &_loader)
    : m_adaptor(new parser::ParserAdaptor),
      m_loader(_loader)
{}

ProjectCalculator::~ProjectCalculator()
{
    delete m_adaptor;
}

double ProjectCalculator::sumAll(const QList<double> &_values)
{
    double ans = 0;
    for(double val : _values)
        ans += val;
    return ans;
}

TreeRightSideValues *ProjectCalculator::getAverageRightSide(ProjectsLoaderPtr &_loader,
                                                            const QString &_leftSide)
{
    TreeLeftSideInfo* leftSide = _loader->getLeftSideInfo(_leftSide);
    Q_ASSERT(leftSide);

    QList<TreeRightSideValues *> rightSides = leftSide->getRightSides();
    QMap<QString, double> averageValues;
    int rightSidesCount = 0;
    for(TreeRightSideValues* leftSide : rightSides)
    {
        if(leftSide->isTemp() || leftSide->id() == "finalCast")
            continue;

        QMap<QString, double> values = leftSide->values();
        QMapIterator<QString, double> iter(values);
        while (iter.hasNext())
        {
            iter.next();
            averageValues[iter.key()] += iter.value();
        }
        ++rightSidesCount;
    }

    QMapIterator<QString, double> iter(averageValues);
    while (iter.hasNext())
    {
        iter.next();
        averageValues[iter.key()] /= rightSidesCount;
    }

    TreeRightSideValues* average = _loader->getOrCreateRightSide(leftSide->treeName(), "average", true);
    average->setValues(averageValues);
    return average;
}

TreeRightSideValues *ProjectCalculator::getFinalCastRightSide(ProjectsLoaderPtr &_loader, const QString &_leftSide)
{
    TreeRightSideValues* average = getAverageRightSide(_loader, _leftSide);
    TreeRightSideValues* finalCast = _loader->getOrCreateRightSide(_leftSide, "finalCast", false);

    QMap<QString, double> finalCastValues = finalCast->values();
    QMap<QString, double> averageValues = average->values();
    QMapIterator<QString, double> finalCastIter(finalCastValues);
    while (finalCastIter.hasNext())
    {
        finalCastIter.next();
        if(finalCastIter.value() == 0)
        {
            const QString& key = finalCastIter.key();
            finalCastValues[key] = averageValues[key];
        }
    }
    finalCast->setValues(finalCastValues);
    return finalCast;
}

TreeRightSideValues *ProjectCalculator::normalise(ProjectsLoaderPtr &_loader, TreeRightSideValues *_values)
{
    QMap<QString, double> values = _values->values();

    double summ = 0;
    for(double d : values)
        summ += d;

    for(double& val : values)
    {
        val /= summ;
    }

    TreeRightSideValues* newVals = _loader->getOrCreateRightSide(_values->leftSideId(), "normalised", true);
    newVals->setValues(values);
    return newVals;
}

void ProjectCalculator::addRangedFactors(QMap<QString, double> &_values)
{
    QMap<QString, double> averageRangedFactors = m_averageRangedFactorsJudges->values();

    QSet<QString> valuesKeys = QSet<QString>::fromList(_values.keys());
    QSet<QString> rangedKeys = QSet<QString>::fromList(averageRangedFactors.keys());

    QSet<QString> intersection = valuesKeys.intersect(rangedKeys);
    qDebug() << Q_FUNC_INFO << " Warning! the following constants are ambigos!:\n" <<
                intersection.toList();

    _values.unite(averageRangedFactors);
}

void ProjectCalculator::calculate()
{
    updateFieldsFromLoader();

    const QString formulsPath = m_loader->formulsPath();

    Q_ASSERT_X(m_constants->savedRightSidesCount() == 2, Q_FUNC_INFO, "it should be 2 constants values files"); // Пока так
    TreeRightSideValues* constants0 = m_constants->openRightSide(0);
    TreeRightSideValues* constants1 = m_constants->openRightSide(1);

    QMap<QString, double> oneProjConstants   = constants0->values();
    QMap<QString, double> otherProjConstants = constants1->values();

    //add ranged factors:
    addRangedFactors(oneProjConstants);
    addRangedFactors(otherProjConstants);
    qDebug() << "Left sides of calculation:\n";
    logInColumns(oneProjConstants, otherProjConstants);

    QMap<QString, double> oneProjectCalculation   = calculateProject(oneProjConstants,   formulsPath);
    QMap<QString, double> otherProjectCalculation = calculateProject(otherProjConstants, formulsPath);

    qDebug() << "AFTER CALCULATION:\n" ;//<< oneProjectCalculation << "\n------\n" << otherProjectCalculation;
    logInColumns(oneProjectCalculation, otherProjectCalculation);

    removeNan(oneProjectCalculation);
    removeNan(otherProjectCalculation);

    normalise(oneProjectCalculation, otherProjectCalculation);
    qDebug() << "\nAFTER NORMALISATION:\n";
    logInColumns(oneProjectCalculation, otherProjectCalculation);

    qDebug() << "\nWEIGHTS:\n" << m_metodicJudgesAverage->values();
    oneProjectCalculation   = multiply(oneProjectCalculation,   m_metodicJudgesAverage->values()); // умножаем на среднее по весам критериев по экспертам
    otherProjectCalculation = multiply(otherProjectCalculation, m_metodicJudgesAverage->values());
    qDebug() << "\nAFTER MULTIPLY ON THE WEIGHTS:\n";
    logInColumns(oneProjectCalculation, otherProjectCalculation);

    qDebug() << "final cast: " << m_sectionsFinalCast->values();
    oneProjectCalculation   = multiplyWithSection(oneProjectCalculation,   m_sectionsFinalCast->values(), m_methodicJudges->nodes());
    otherProjectCalculation = multiplyWithSection(otherProjectCalculation, m_sectionsFinalCast->values(), m_methodicJudges->nodes());
    qDebug() << "\nAFTER MULTIPLY ON THE SECTIONS FINAL CAST:\n";
    logInColumns(oneProjectCalculation, otherProjectCalculation);

    TreeRightSideValues* result0 = m_result->openRightSide(0);
    TreeRightSideValues* result1 = m_result->openRightSide(1);
    result0->setValues(oneProjectCalculation);
    result1->setValues(otherProjectCalculation);
}

void ProjectCalculator::updateSectionCalculation(ProjectsLoaderPtr &_loader)
{
    auto summorizeFactorsInSections = [&_loader](const QMap<QString, double>& _calculatedVals)
    {
        QMap<QString, double> ans;
        TreeLeftSideInfo* methodicJudges = _loader->getLeftSideInfo(serializeConstants::metodicJudges);

        for (PropertyNode* node : methodicJudges->nodes())
        {
            QString groupKey = node->key();
            ans[groupKey] = 0;
            for (PropertyNode* subNode : node->children())
            {
                QString subNodeKey = subNode->key();
                double subNodeValue = _calculatedVals[subNodeKey];
                ans[groupKey] += subNodeValue;
            }
        }
        return ans;
    };

    TreeLeftSideInfo *resultLeftSide = _loader->getLeftSideInfo(serializeConstants::result);
    TreeLeftSideInfo *resultSectionsLeftSide = _loader->getLeftSideInfo(serializeConstants::sectionsResult);

    auto resultRightSides = resultLeftSide->getRightSides();
    auto resultSectionsRightSides = resultSectionsLeftSide->getRightSides();

    Q_ASSERT(resultRightSides.size() == resultSectionsRightSides.size());
    int size = resultRightSides.size();

    for(int i = 0; i < size; ++i)
    {
        TreeRightSideValues* items = resultRightSides.at(i);
        TreeRightSideValues* sections = resultSectionsRightSides.at(i);

        auto values = summorizeFactorsInSections(items->values());
        sections->setValues(values);
    }
}

void ProjectCalculator::normalise(QMap<QString, double> _values1, QMap<QString, double> _values2)
{
    QMapIterator<QString, double> i(_values1);
    while (i.hasNext())
    {
        i.next();
        QString key = i.key();
        normalise(_values1[key], _values2[key]);
    }
}

void ProjectCalculator::normalise(double &_one, double &_other)
{
    double max = qMax(_one, _other);
    if(max == 0)
    {
        _one = 1;
        _other = 1;
        return;
    }
    _one /= max;
    _other /= max;
}

QMap<QString, double> ProjectCalculator::calculateProject(const QMap<QString, double> &_source, const QString &_formulsFilename)
{
    QMap<QString, double> ans;
    QFile file(_formulsFilename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "no file " << _formulsFilename;
        return ans;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().simplified();
        if(line.isEmpty())
            continue;

        int equalSignPos = line.indexOf("=");
        QString lSide = line.left(equalSignPos).simplified();

        int equalSignPosFromRight = line.size() - equalSignPos - 1;
        line = line.right(equalSignPosFromRight).simplified();
        line = substitute(line, _source);

        QString calculated = m_adaptor->calculate(line);
        double calculatedMean = toDouble(calculated);

        if(std::isnan(calculatedMean) || std::isinf(calculatedMean))
            qDebug() << "wrong calculated" << lSide << "=" << line;
        ans[lSide] = calculatedMean;
    }
    return ans;
}

QString ProjectCalculator::substitute(const QString &_expression, const QMap<QString, double> &_source)
{
    QString ans = _expression;
    QStringList rSide = _expression.split(QRegExp("[\\s+=*/()]"), QString::SkipEmptyParts);

    for(QString operand : rSide)
    {
        static QRegExp variableMatcher("^[a-zA-Z_][a-zA-Z0-9_]*$");
        bool isVariable = operand.contains(variableMatcher);
        if(!isVariable)
        {
            static QRegExp constantsMatcher("^[-+]?[0-9]*\\.?[0-9]*$");
            if(!operand.contains(constantsMatcher))
            {
                qDebug() << "Wrong argument passed in calculator:" << operand;
                Q_ASSERT(false);
            }
            continue;
        }

        if(!_source.contains(operand))
        {
            qDebug() << "WARNING! There is no tag with name " << operand;
            break; // !!!
        }
        double operandMean = _source[operand];
        ans.replace(operand, QString::number(operandMean));
    }

    return ans;
}

void ProjectCalculator::logInColumns(const QMap<QString, double> &_oneProject, const QMap<QString, double> &_otherProject)
{
    Q_ASSERT(_oneProject.keys() == _otherProject.keys());
    QMapIterator<QString, double> k(_oneProject);
    while (k.hasNext())
    {
        k.next();
        QString key = k.key();
        qDebug() << key << "\t" << _oneProject[key] << "\t" << _otherProject[key];
    }
}

void ProjectCalculator::updateFieldsFromLoader()
{
    m_metodicJudgesAverage =
            ProjectCalculator::getAverageRightSide(m_loader, serializeConstants::metodicJudges);
    m_averageRangedFactorsJudges =
            ProjectCalculator::getAverageRightSide(m_loader, serializeConstants::rangedFactorsJudges);
    m_sectionsFinalCast =
            ProjectCalculator::getFinalCastRightSide(m_loader, serializeConstants::sections);

    m_loader->getOrCreateRightSide(serializeConstants::result, "result0");
    m_loader->getOrCreateRightSide(serializeConstants::result, "result1");

    m_methodicJudges = m_loader->getLeftSideInfo(serializeConstants::metodicJudges);

    m_rangedFactorsJudges = m_loader->getLeftSideInfo(serializeConstants::rangedFactorsJudges);
    m_constants           = m_loader->getLeftSideInfo(serializeConstants::constants);
    m_result              = m_loader->getLeftSideInfo(serializeConstants::result);
}

QMap<QString, double> multiply(const QMap<QString, double>& _one, const QMap<QString, double>& _other)
{
    QMap<QString, double> ans;

    QMapIterator<QString, double> i(_one);
    while (i.hasNext())
    {
        i.next();
        const QString& key = i.key();
        ans[key] = _one[key] * _other[key];
    }
    return ans;
}

QMap<QString, double> multiplyWithSection(const QMap<QString, double>& _values, const QMap<QString, double>& _sections, const QList<PropertyNode*>& _nodes)
{
    QMap<QString, double> ans;
    for(int i = 0; i < _nodes.size(); ++i)
    {
        PropertyNode* titleNode = _nodes.at(i);
        QString titleKey = titleNode->key();
        double groupKoef = _sections[titleKey];

        for(PropertyNode* node : titleNode->children())
        {
            QString key = node->key();
            ans[key] = _values[key] * groupKoef;
        }
    }
    return ans;
}

void removeNan(QMap<QString, double>& _values)
{
    QMapIterator<QString, double> iter(_values);
    while (iter.hasNext())
    {
        iter.next();
        QString key = iter.key();
        double val = _values[key];
        if(std::isnan(val) || std::isinf(val))
            _values[key] = 0;
    }
}
