#include <QDebug>
#include <QStringList>

#include "projectapi.h"
#include "projectcalculation.h"
#include "nodesinfo/treeinfofactory.h"
#include "nodesinfo/treerightsidevalues.h"

QList<double> calculateProject(QList<double> _source);

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

void ProjectCalculator::calculate(const QString &_metodicJudgesAverage, const QString &_sectionsAverage, const QString &_oneProject, const QString &_otherProject, const QString &_baseResultName)
{
    TreeInfoFactory* factory = new TreeInfoFactory();
    TreeRightSideValues* methodicAverageVals    = factory->getRightSideValues(_metodicJudgesAverage);
    TreeRightSideValues* sectionsVals           = factory->getRightSideValues(_sectionsAverage);
    TreeRightSideValues* oneProjConstantsVals   = factory->getRightSideValues(_oneProject);
    TreeRightSideValues* otherProjConstantsVals = factory->getRightSideValues(_otherProject);

    QList<double> methodicAv          = toDoubleList(methodicAverageVals->values());
    QList<double> sectionsAv          = toDoubleList(sectionsVals->values());
    QList<double> oneProjConstants    = toDoubleList(oneProjConstantsVals->values());
    QList<double> otherProjConstants  = toDoubleList(otherProjConstantsVals->values());
    Q_ASSERT(oneProjConstants.size() == otherProjConstants.size());

    QList<double> oneProjectCalculation   = calculateProject(oneProjConstants);
    QList<double> otherProjectCalculation = calculateProject(otherProjConstants);

    qDebug() << "AFTER CALCULATION:\n" << oneProjectCalculation << "\n------\n" << otherProjectCalculation;

    for(int i = 0; i < oneProjectCalculation.size(); ++i)
        normalise(oneProjectCalculation[i], otherProjectCalculation[i]);

    qDebug() << "\nAFTER NORMALISATION:\n" << oneProjectCalculation << "\n--------\n" << otherProjectCalculation;

    oneProjectCalculation   = multiply(oneProjectCalculation,   methodicAv);
    otherProjectCalculation = multiply(otherProjectCalculation, methodicAv);

    oneProjectCalculation   = multiplyByGroups(oneProjectCalculation,   sectionsAv);
    otherProjectCalculation = multiplyByGroups(otherProjectCalculation, sectionsAv);

    oneProjConstantsVals->setValues(toVariantList(oneProjectCalculation));
    otherProjConstantsVals->setValues(toVariantList(otherProjectCalculation));

    oneProjConstantsVals->writeValues(_baseResultName + QString::number(0));
    otherProjConstantsVals->writeValues(_baseResultName + QString::number(1));
    delete factory;
}

QList<int> ProjectCalculator::groupTitles()
{
    static QList<int> titles;
    if(titles.isEmpty())
        titles << 0 << 12 << 14 << 31 << 33 << 36 << 38 << 43 << 61;

    return titles;
}

QList<double> calculateProject(QList<double> _source)
{
    QVector <double> ans;
    ans.resize(58);

    ans[0]  =  _source[0] / _source[1] + 1;
    ans[1]  =  _source[2] / _source[3];
    ans[2]  =  _source[4] / _source[5];
    ans[3]  = (_source[6] + _source[7]  + _source[8]) /
              (_source[9] + _source[10] + _source[11]) ;

    if(_source[13] == 0)
        ans[4] = 0;
    else
        ans[4]  = _source[12] / _source[13];
    ans[5]  = _source[14] / _source[15];
    ans[6]  = _source[16] / _source[17];
    ans[7]  =(_source[18] + _source[19] + _source[20]) /
             (_source[21] + _source[22] + _source[23]) ;
    ans[8]  = _source[24] / _source[25];
    ans[9]  = _source[26] / _source[27]; // эта строчка была добавлена
    ans[10]	= _source[28] / _source[29];
    ans[11]	= _source[30] / _source[31];
    ans[12]	= _source[32] / _source[33];
    ans[13]	= _source[34] / _source[35];
    ans[14]	= _source[36] / _source[37];
    ans[15]	= _source[38] / _source[39];
    ans[16]	= _source[40] * _source[41] / _source[42]; // Количество создаваемых (сохраняемых) рабочих мест  в смежных отраслях
    ans[17]	= _source[43] + _source[44] + _source[45];
    ans[18]	= _source[46] - _source[47]; // Наличие трудовых ресурсов соответствующей квалификации. Внесено изменение с методикой.
    ans[19]	= _source[48] + _source[49] + _source[50];
    ans[20]	=(_source[51] + _source[52] + _source[53])/ _source[54];
    ans[21]	= _source[55] + _source[56] + _source[57] + _source[58];
    ans[22]	= _source[59] * _source[60] * _source[61]; // Сокращение времени пребывания пассажиров в пути (увеличение агломерационного радиуса)
    ans[23]	= _source[62] / _source[63];
    ans[24]	= _source[64] / _source[65];
    ans[25]	= _source[66] + _source[67] + _source[68] + _source[69];
    ans[26]	= _source[70] + _source[71] + _source[72];
    ans[27]	= 1140 * _source[74] / _source[73] +
             (_source[75] * _source[76] * _source[77]) / _source[78]; // train + auto. Увеличение пропускной способности путей сообщения
    ans[28]	= _source[79] / _source[80];
    ans[29] = _source[81] / _source[82];
    ans[30]	= _source[83] / _source[84]; // Платежная способность населения в регионе
    ans[31]	= _source[85] / _source[86];
    ans[32]	= _source[87] + _source[88] + _source[89] + _source[90];
    ans[33]	= _source[91] + _source[92] + _source[93];
    ans[34]	= _source[94] / _source[95] + (_source[96] * _source[97]) / _source[98];//
    ans[35]	= _source[99] / _source[100]; // Изменение населенности подвижного состава


    for(int i = 36; i <= 57; i++)
    {
       ans[i] = _source[i + 63];
    }

    for(int i = 0; i < ans.size(); ++i)
    {
        if(std::isnan(ans[i]) || std::isinf(ans[i]))
        {
            qDebug() << "WARNING! ans[" << i << "] is nan!!!";
            ans[i] = 0;
        }
    }



    return ans.toList();
}

QList<double> ProjectCalculator::multiply(const QList<double>& _values, const QList<double>& _average)
{
    QList<double> ans;
    int size = _values.size();
    int count = 0;
    int j = 0;
    for(int i = 0; i < size; ++i, ++j)
    {
        if(groupTitles().at(count) == i)
        {
            ++count;
            ++j;
        }
        ans << _values.at(i) * _average.at(j);
    }

    return ans;
}

QList<double> ProjectCalculator::multiplyByGroups(const QList<double>& _vals, const QList<double>& _groupsKoeff)
{
    QList<double> ans;

    QList<int> intervals = groupTitles();

    for(int i = 0; i < intervals.size() - 1; ++i)
    {
        int left = intervals.at(i);
        int right = intervals.at(i + 1);
        for(int j = left - i; j < right - i; ++j)
        {
            ans << _groupsKoeff.at(i) * _vals.at(j);
        }
    }
    return ans;
}
