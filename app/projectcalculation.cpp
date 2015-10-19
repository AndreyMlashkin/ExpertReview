#include <QDebug>
#include <QStringList>

#include "projectapi.h"
#include "projectcalculation.h"
#include "nodesinfo/treeleftsideinfo.h"
#include "nodesinfo/treerightsidevalues.h"
#include "properynode.h"

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
    : m_methodicJudges(_methodicJudges),
      m_metodicJudgesAverage(_metodicJudgesAverage),
      m_sectionsAverage(_sectionsAverage)
{}

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

    qDebug() << "AFTER CALCULATION:\n" << oneProjectCalculation << "\n------\n" << otherProjectCalculation;

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

// по-новому
QMap<QString, double> calculateProject(const QMap<QString, double>& _source)
{
    QMap<QString, double> ans;

    ans["dinNDSpred"]    = 1 + _source["NDS_pr"] / _source["NDS_tek"];
    ans["dinNPpred"]     = 1 + _source["NP_pr"]  / _source["NP_tek"];
    ans["dinNIpred"]     = 1 + _source["NI_pr"]  / _source["NI_tek"];
    ans["dinVneBUDpred"] = 1 + (_source["PF_pr"]  + _source["FSS_pr"]  + _source["OMS_pr"]) /
                               (_source["PF_tek"] + _source["FSS_tek"] + _source["OMS_tek"]);

    ans["dinNDS"] = (_source["NDS_tek_E"] == 0)? 0 : 1 + _source["NDS_pr_E"] / _source["NDS_tek_E"];

    ans["dinNP"]         = 1 + _source["NP_pr_E"]  / _source["NP_tek_E"];
    ans["dinNI"]         = 1 + _source["NI_pr_E"]  / _source["NI_tek_E"];
    ans["dinVneBUD"]     = 1 + (_source["PF_pr_E"]  + _source["FSS_pr_E"]  + _source["OMS_pr_E"]) /
                               (_source["PF_tek_E"] + _source["FSS_tek_E"] + _source["OMS_tek_E"]);
    ans["dinNDFLtransp"] = 1 + _source["NDFL_pr_TrE"] / _source["NDFL_tek_TrE"];
    ans["dinNDFLkosv"]   = 1 + _source["NDFL_pr_KE"]  / _source["NDFL_tek_KE"];
    ans["dinZemN"]       = 1 + _source["NZ_pr_E"]     / _source["NZ_tek_E"];

    ans["doljaPredpr"]   = 1 + _source["PPd"]      / _source["PP"];
    ans["uslugiTovar"]   = 1 + _source["NP_pr_SE"] / _source["NP_tek_SE"];
    ans["stoim.1kv.m"]   = 1 + _source["Ned_pr"]   / _source["Ned_tek"];
    ans["rabMestTr"]     = 1 + _source["PMt_pr"]   / _source["PMt_tek"];
    ans["rabMestSm"]     =  _source["_I"]  * _source["_d"]  / _source["_S"]; //  Количество создаваемых (сохраняемых) рабочих мест  в смежных отраслях
    ans["stoimRes"]      =  _source["_Sm"] + _source["Str"] + _source["Scmr"];
    ans["trudRes"]       = 1 + _source[""] / _source[""]; //  Наличие трудовых ресурсов соответствующей квалификации. Внесено изменение с методикой.
    ans["passPotok"]     =  _source["POprr"] + _source["POpra"] + _source["POprb"];
    ans["podvizhnNasel"] = (_source["Pt"] + _source["Po"] + _source["Pn"]) / _source["Chislenn"];
    ans["otechProd"]     =  _source["IZsps"] + _source["IZsi"] + _source["IZeps"] + _source["IZei"];
    ans["vremjaPuti"]    =  _source["Ttr"] * _source["Sp"] * _source["Knp"];
    ans["stoimZemli"]    =  _source["Zem_pr"] / _source["Zem_tek"];
    ans["naselPunkt"]    =  _source["NPd"] / _source["NP"];
    ans["innovProd"]     =  _source["IPsps"] + _source["IPsi"] + _source["IPeps"] + _source["IPei"];
    ans["zemleotvod"]    =  _source["Srs"] + _source["Spk"] + _source["Sob"];
    ans["propuskSpos"]   =  1140 * _source["k"] / _source["T"] +
                           (_source["lt"] * _source["lp"] * _source["la"]) / _source["U"]; // train + auto. Увеличение пропускной способности путей сообщения

    ans["dinKoefInfr"]     = _source["POchp"] / _source["POpr"];

    ans["dinDostup"]       = _source["Z"] / _source["pass_km"];
    ans["platezhespos"]    = _source["Sb"] / _source["Zsrm"]; // платежная способность населения в регионе

    ans["kachestvoUslug"]  = _source["L"] / _source["S"];
    ans["dohodPerev"]      = _source["Dprr"] + _source["Dpra"] + _source["Dprch"] + _source["Dprb"];
    ans["tOborota"]        = _source["Tto"] + _source["Tpv"] + _source["Tdv"];
    ans["podvSostav"]      = _source["Qsut"] / _source["Urd"] + (_source["Qmax"] * _source["Tob"]) / _source["qvm"];
    ans["naselPodvSostav"] = _source["Pkm"] / _source["Vkm"]; // Изменение населенности подвижного состава

    ans["riskTehnPrir"]      = _source["rTehnPrir"];
    ans["riskNacCrizis"]     = _source["rNacCrizis"];
    ans["riskVVP"]           = _source["rVVP"];
    ans["riskInfl"]          = _source["rInfl"];
    ans["riskBezrab"]        = _source["rBezrab"];
    ans["riskNalog"]         = _source["rNalog"];
    ans["riskKonkur"]        = _source["rKonkur"];
    ans["riskIzmPredp"]      = _source["rIzmPredp"];
    ans["riskZaderzh"]       = _source["rZaderzh"];
    ans["riskOperac"]        = _source["rOperac"];
    ans["riskStoim_Zaderzh"] = _source["rStoim_Zaderzh"];
    ans["riskFinans"]        = _source["rFinans"];
    ans["riskPostavok"]      = _source["rPostavok"];
    ans["riskEcologii"]      = _source["rEcologii"];
    ans["riskChPStroit"]     = _source["rChPStroit"];
    ans["riskStroitNorm"]    = _source["rStroitNorm"];
    ans["riskForceMajor"]    = _source["rForceMajor"];

    ans["riskShum"]     = _source["rShum"];
    ans["riskVibro"]    = _source["rVibro"];
    ans["riskIzluch"]   = _source["rIzluch"];
    ans["riskZagrVoda"] = _source["rZagrVoda"];
    ans["riskZagrVozd"] = _source["rZagrVozd"];
    ans["riskMusor"]    = _source["rMusor"];

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
            _values[key] = -1;
    }
}
