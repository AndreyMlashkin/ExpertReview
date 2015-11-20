#include <QDebug>
#include <QStringList>
#include <QFile>

#include "projectapi.h"
#include "projectcalculation.h"
#include "nodesinfo/treeleftsideinfo.h"
#include "nodesinfo/treerightsidevalues.h"
#include "properynode.h"
#include "../expressioncalculator/parseradaptor.h"

QMap<QString, double> calculateProject(const QMap<QString, double>& _source);
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
{
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
            static QRegExp constantsMatcher("^[-+]?[0-9]*\.?[0-9]*$");
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
/*QMap<QString, double> calculateProject(const QMap<QString, double>& _source)
{
    auto getValue = [&_source](const QString& _key)
    {
        bool haveValue = _source.contains(_key);
        if(!haveValue)
            qDebug() << "WARNING! There is no tag with name " << _key;
        return _source[_key];
    };

    QMap<QString, double> ans;

    ans["dinNDSpred"]    = 1 + getValue("NDS_pr") / getValue("NDS_tek");
    ans["dinNPpred"]     = 1 + getValue("NP_pr")  / getValue("NP_tek");
    ans["dinNIpred"]     = 1 + getValue("NI_pr")  / getValue("NI_tek");
    ans["dinVneBUDpred"] = 1 + (getValue("PF_pr")  + getValue("FSS_pr")  + getValue("OMS_pr")) /
                               (getValue("PF_tek") + getValue("FSS_tek") + getValue("OMS_tek"));

    ans["dinNDS"]        = 1 + getValue("NDS_pr_E") / getValue("NDS_tek_E");

    ans["dinNP"]         = 1 + getValue("NP_pr_E")  / getValue("NP_tek_E");
    ans["dinNI"]         = 1 + getValue("NI_pr_E")  / getValue("NI_tek_E");
    ans["dinVneBUD"]     = 1 + (getValue("PF_pr_E")  + getValue("FSS_pr_E")  + getValue("OMS_pr_E")) /
                               (getValue("PF_tek_E") + getValue("FSS_tek_E") + getValue("OMS_tek_E"));
    ans["dinNDFLtransp"] = 1 + getValue("NDFL_pr_TrE") / getValue("NDFL_tek_TrE");
    ans["dinNDFLkosv"]   = 1 + getValue("NDFL_pr_KE")  / getValue("NDFL_tek_KE");
    ans["dinZemN"]       = 1 + getValue("NZ_pr_E")     / getValue("NZ_tek_E");

    ans["doljaPredpr"]   = 1 + getValue("PPd")      / getValue("PP");
    ans["uslugiTovar"]   = 1 + getValue("NP_pr_SE") / getValue("NP_tek_SE");
    ans["stoim.1kv.m"]   = 1 + getValue("Ned_pr")   / getValue("Ned_tek");
    ans["rabMestTr"]     = 1 + getValue("PMt_pr")   / getValue("PMt_tek");
    ans["rabMestSm"]     =  getValue("_I")  * getValue("_d")  / getValue("_S"); //  Количество создаваемых (сохраняемых) рабочих мест  в смежных отраслях
    ans["stoimRes"]      =  getValue("_Sm") + getValue("Str") + getValue("Scmr");
//    ans["trudRes"]       = 1 + _source[""] / _source[""]; //  Наличие трудовых ресурсов соответствующей квалификации. Внесено изменение с методикой.
    ans["passPotok"]     =  getValue("POprr") + getValue("POpra") + getValue("POprb");
    ans["podvizhnNasel"] = (getValue("Pt") + getValue("Po") + getValue("Pn")) / getValue("Chislenn");
    ans["otechProd"]     =  getValue("IZsps") + getValue("IZsi") + getValue("IZeps") + getValue("IZei");
    ans["vremjaPuti"]    =  getValue("Ttr") * getValue("Sp") * getValue("Knp");
    ans["stoimZemli"]    =  getValue("Zem_pr") / getValue("Zem_tek");
    ans["naselPunkt"]    =  getValue("NPd") / getValue("Np");
    ans["innovProd"]     =  getValue("IPsps") + getValue("IPsi") + getValue("IPeps") + getValue("IPei");
    ans["zemleotvod"]    =  getValue("Srs") + getValue("Spk") + getValue("Sob");
    ans["propuskSpos"]   =  1140 * getValue("k") / getValue("T") +
                           (getValue("lt") * getValue("lp") * getValue("la")) / getValue("U"); // train + auto. Увеличение пропускной способности путей сообщения

    ans["dinKoefInfr"]     = getValue("POchp") / getValue("POpr");

    ans["dinDostup"]       = getValue("Z") / getValue("pass_km");
    ans["platezhespos"]    = getValue("Sb") / getValue("Zsrm"); // платежная способность населения в регионе

    ans["kachestvoUslug"]  = getValue("L") / getValue("S");
    ans["dohodPerev"]      = getValue("Dprr") + getValue("Dpra") + getValue("Dprch") + getValue("Dprb");
    ans["tOborota"]        = getValue("Tto") + getValue("Tpv") + getValue("Tdv");
    ans["podvSostav"]      = getValue("Qsut") / getValue("Urd") + (getValue("Qmax") * getValue("Tob")) / getValue("qvm");
    ans["naselPodvSostav"] = getValue("Pkm") / getValue("Vkm"); // Изменение населенности подвижного состава

    ans["riskTehnPrir"]      = getValue("rTehnPrir");
    ans["riskNacCrizis"]     = getValue("rNacCrizis");
    ans["riskVVP"]           = getValue("rVVP");
    ans["riskInfl"]          = getValue("rInfl");
    ans["riskBezrab"]        = getValue("rBezrab");
    ans["riskNalog"]         = getValue("rNalog");
    ans["riskKonkur"]        = getValue("rKonkur");
    ans["riskIzmPredp"]      = getValue("rIzmPredp");
    ans["riskZaderzh"]       = getValue("rZaderzh");
//    ans["riskOperac"]        = getValue("rOperac");
    ans["riskStoim_Zaderzh"] = getValue("rStoim_Zaderzh");
    ans["riskFinans"]        = getValue("rFinans");
    ans["riskPostavok"]      = getValue("rPostavok");
    ans["riskEcologii"]      = getValue("rEcologii");
    ans["riskChPStroit"]     = getValue("rChPStroit");
    ans["riskStroitNorm"]    = getValue("rStroitNorm");
    ans["riskForceMajor"]    = getValue("rForceMajor");

    ans["riskShum"]     = getValue("rShum");
    ans["riskVibro"]    = getValue("rVibro");
    ans["riskIzluch"]   = getValue("rIzluch");
    ans["riskZagrVoda"] = getValue("rZagrVoda");
    ans["riskZagrVozd"] = getValue("rZagrVozd");
    ans["riskMusor"]    = getValue("rMusor");

    return ans;
}*/

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
            _values[key] = -1;
    }
}
