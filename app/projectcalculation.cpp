#include <QDebug>
#include <QStringList>
#include <QFile>

#include <include/expressioncalculator/parseradaptor.h>
#include "projectapi.h"
#include "projectcalculation.h"
#include "nodesinfo/treeleftsideinfo.h"
#include "nodesinfo/treerightsidevalues.h"
#include "properynode.h"

QMap<QString, double> multiply(const QMap<QString, double>& _one, const QMap<QString, double>& _other);
QMap<QString, double> multiplyWithSection(const QMap<QString, double>& _values, const QMap<QString, double>& _sections, const QList<ProperyNode*>& _nodes);

void removeNan(QMap<QString, double>& _values);

inline void normalise(double& _one, double& _other)
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

void ProjectCalculator::calculate(TreeLeftSideInfo *_source, TreeLeftSideInfo *_result)
{
    Q_ASSERT(_source->savedRightSidesCount() == 2); // Пока так
    TreeRightSideValues* values0 = _source->openRightSide(0);
    TreeRightSideValues* values1 = _source->openRightSide(1);

    TreeRightSideValues* result0 = _result->createRightSide();
    TreeRightSideValues* result1 = _result->createRightSide();

    calculate(values0, values1, result0, result1);
}

void ProjectCalculator::calculate(TreeRightSideValues *_oneProject, TreeRightSideValues *_otherProject, TreeRightSideValues *_result0, TreeRightSideValues *_result1)
{
    QMap<QString, double> oneProjConstants   = _oneProject->values();
    QMap<QString, double> otherProjConstants = _otherProject->values();

    QMap<QString, double> oneProjectCalculation   = calculateProject(oneProjConstants);
    QMap<QString, double> otherProjectCalculation = calculateProject(otherProjConstants);

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

    oneProjectCalculation   = multiply(oneProjectCalculation,   m_metodicJudgesAverage->values());
    otherProjectCalculation = multiply(otherProjectCalculation, m_metodicJudgesAverage->values());

    oneProjectCalculation   = multiplyWithSection(oneProjectCalculation,   m_sectionsAverage->values(), m_methodicJudges->nodes());
    otherProjectCalculation = multiplyWithSection(otherProjectCalculation, m_sectionsAverage->values(), m_methodicJudges->nodes());

    _result0->setValues(oneProjectCalculation);
    _result1->setValues(otherProjectCalculation);

    //!!!
    _result0->writeValues("result0");
    _result1->writeValues("result1");
}

QMap<QString, double> ProjectCalculator::calculateProject(const QMap<QString, double> &_source)
{
    QMap<QString, double> ans;
    QFile file("formuls.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "no file formuls.txt";
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

QMap<QString, double> multiplyWithSection(const QMap<QString, double>& _values, const QMap<QString, double>& _sections, const QList<ProperyNode*>& _nodes)
{
    QMap<QString, double> ans;
    for(int i = 0; i < _nodes.size(); ++i)
    {
        ProperyNode* titleNode = _nodes.at(i);
        QString titleKey = titleNode->key();
        double groupKoef = _sections[titleKey];

        foreach(ProperyNode* node, titleNode->children())
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
