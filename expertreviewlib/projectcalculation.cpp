#include <QDebug>
#include <QStringList>
#include <QFile>

#include <parseradaptor.h>
#include "projectapi.h"
#include "projectcalculation.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"
#include "serialization/nodesinfo/treerightsidevalues.h"
#include "serialization/nodesinfo/propertynode.h"

QMap<QString, double> multiply(const QMap<QString, double>& _one, const QMap<QString, double>& _other);
QMap<QString, double> multiplyWithSection(const QMap<QString, double>& _values, const QMap<QString, double>& _sections, const QList<PropertyNode*>& _nodes);

void removeNan(QMap<QString, double>& _values);

ProjectCalculator::ProjectCalculator(TreeLeftSideInfo* _methodicJudges, TreeRightSideValues *_metodicJudgesAverage, TreeRightSideValues *_sectionsAverage)
    : m_adaptor(new parser::ParserAdaptor),
      m_methodicJudges(_methodicJudges),
      m_metodicJudgesAverage(_metodicJudgesAverage),
      m_sectionsAverage(_sectionsAverage)
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

    QList<TreeRightSideValues *> leftSides = leftSide->getRightSides();
    QMap<QString, double> averageValues;
    for(TreeRightSideValues* leftSide : leftSides)
    {
        QMap<QString, double> values = leftSide->values();
        QMapIterator<QString, double> iter(values);
        while (iter.hasNext())
        {
            iter.next();
            averageValues[iter.key()] += iter.value();
        }
    }

    QMapIterator<QString, double> iter(averageValues);
    while (iter.hasNext())
    {
        iter.next();
        averageValues[iter.key()] /= leftSides.count();
    }

    TreeRightSideValues* average = _loader->getOrCreateRightSide(leftSide->treeName(), "average", true);
    average->setValues(averageValues);
    return average;
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

void ProjectCalculator::calculate(TreeLeftSideInfo *_source, TreeLeftSideInfo *_result,
                                  const QString& _formulsPath)
{
    Q_ASSERT_X(_source->savedRightSidesCount() == 2, Q_FUNC_INFO, "it should be 2 constants values files"); // Пока так
    TreeRightSideValues* values0 = _source->openRightSide(0);
    TreeRightSideValues* values1 = _source->openRightSide(1);

    TreeRightSideValues* result0 = _result->openRightSide(0);
    TreeRightSideValues* result1 = _result->openRightSide(1);

    calculate(values0, values1, result0, result1, _formulsPath);
}

void ProjectCalculator::calculate(TreeRightSideValues *_oneProject, TreeRightSideValues *_otherProject,
                                  TreeRightSideValues *_result0,    TreeRightSideValues *_result1,
                                  const QString &_formulsPath)
{
    QMap<QString, double> oneProjConstants   = _oneProject->values();
    QMap<QString, double> otherProjConstants = _otherProject->values();

    QMap<QString, double> oneProjectCalculation   = calculateProject(oneProjConstants,   _formulsPath);
    QMap<QString, double> otherProjectCalculation = calculateProject(otherProjConstants, _formulsPath);

    qDebug() << "AFTER CALCULATION:\n" ;//<< oneProjectCalculation << "\n------\n" << otherProjectCalculation;
    QMapIterator<QString, double> k(oneProjectCalculation);
    while (k.hasNext())
    {
        k.next();
        QString key = k.key();
        qDebug() << key << "\t" << oneProjectCalculation[key] << "\t" << otherProjectCalculation[key];
    }

    removeNan(oneProjectCalculation);
    removeNan(otherProjectCalculation);

    QMapIterator<QString, double> i(oneProjectCalculation);
    while (i.hasNext())
    {
        i.next();
        QString key = i.key();
        normalise(oneProjectCalculation[key], otherProjectCalculation[key]);
    }

    qDebug() << "\nAFTER NORMALISATION:\n" << oneProjectCalculation << "\n--------\n" << otherProjectCalculation;

    oneProjectCalculation   = multiply(oneProjectCalculation,   m_metodicJudgesAverage->values()); // умножаем на среднее по весам критериев по экспертам
    otherProjectCalculation = multiply(otherProjectCalculation, m_metodicJudgesAverage->values());

    oneProjectCalculation   = multiplyWithSection(oneProjectCalculation,   m_sectionsAverage->values(), m_methodicJudges->nodes());
    otherProjectCalculation = multiplyWithSection(otherProjectCalculation, m_sectionsAverage->values(), m_methodicJudges->nodes());

    _result0->setValues(oneProjectCalculation);
    _result1->setValues(otherProjectCalculation);

    //!!!
//    _result0->writeValues("result0");
//    _result1->writeValues("result1");
}

void ProjectCalculator::updateSectionCalculation(ProjectsLoaderPtr &_loader)
{
    auto calculateSections = [&_loader](const QMap<QString, double>& _calculatedVals)
    {
        QMap<QString, double> ans;
        TreeLeftSideInfo* methodicJudges = _loader->getLeftSideInfo("metodicJudges");

        foreach (PropertyNode* node, methodicJudges->nodes())
        {
            QString groupKey = node->key();
            ans[groupKey] = 0;
            foreach (PropertyNode* subNode, node->children())
            {
                QString subNodeKey = subNode->key();
                double subNodeValue = _calculatedVals[subNodeKey];
                ans[groupKey] += subNodeValue;
            }
        }
        return ans;
    };

    TreeLeftSideInfo *resultLeftSide = _loader->getLeftSideInfo("result");
    TreeLeftSideInfo *resultSectionsLeftSide = _loader->getLeftSideInfo("sectionsResult");

    auto resultRightSides = resultLeftSide->getRightSides();
    auto resultSectionsRightSides = resultSectionsLeftSide->getRightSides();

    Q_ASSERT(resultRightSides.size() == resultSectionsRightSides.size());
    int size = resultRightSides.size();

    for(int i = 0; i < size; ++i)
    {
        TreeRightSideValues* items = resultRightSides.at(i);
        TreeRightSideValues* sections = resultSectionsRightSides.at(i);

        auto values = calculateSections(items->values());
        sections->setValues(values);
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

QMap<QString, double> multiply(const QMap<QString, double>& _one, const QMap<QString, double>& _other)
{
    QMap<QString, double> ans;

    QMapIterator<QString, double> i(_one);
    while (i.hasNext())
    {
        i.next();
        QString key = i.key();
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

        foreach(PropertyNode* node, titleNode->children())
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
