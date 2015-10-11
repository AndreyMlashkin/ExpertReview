#include "projectsourcedata.h"
#include "ui_projectsourcedata.h"
#include "treepropertywidget.h"
#include "nodesinfo/treeinfofactory.h"

ProjectSourceData::ProjectSourceData(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProjectSourceData),
    m_factory(new TreeInfoFactory()),
    m_tree(new TreePropertyWidget(m_factory->getLeftSideInfo("constants.txt"), m_factory))
{
    m_ui->setupUi(this);
    m_ui->verticalLayout->addWidget(m_tree);
//    m_tree->setCurrentJudge(QString("constants"), false);

    connect(m_ui->calculate, SIGNAL(clicked()), SLOT(callCalculation()));

}

ProjectSourceData::~ProjectSourceData()
{
//    m_tree->valuesToFile(m_tree->currentJudge(), QString("sourceData.txt"));
    delete m_ui;
}

void ProjectSourceData::callCalculation()
{
/*    QList<double> vals = calculateProject(m_tree->values());

    TreeLeftSideInfo* info = m_factory->getLeftSideInfo("factors.txt");
    TreePropertyWidget* calculation = new TreePropertyWidget(info);
    calculation->setValues("Project1", vals);
    calculation->setCurrentJudge("Project1", false);
//    calculation->showMaximized();
    calculation->show();*/
}

QList<double> ProjectSourceData::calculateProject(QList<double> _source) const
{          
    QVector <double> ans;
    ans.resize(58);

    ans[0]  =  _source[0] / _source[1];
    ans[1]  =  _source[2] / _source[3];
    ans[2]  =  _source[4] / _source[5];
    ans[3]  = (_source[6] + _source[7]  + _source[8]) /
              (_source[9] + _source[10] + _source[11]) ;
    ans[4]  = _source[12] / _source[13];
    ans[5]  = _source[14] / _source[15];
    ans[6]  = _source[16] / _source[17];
    ans[7]  =(_source[18] + _source[19] + _source[20]) /
             (_source[21] + _source[22] + _source[23]) ;
    ans[8]  = _source[24] / _source[25];
    ans[10] = _source[26] / _source[27];
    ans[11] = _source[28] / _source[29];
    ans[12] = _source[30] / _source[31];
    ans[13] = _source[32] / _source[33];
    ans[14] = _source[34] / _source[35];
    ans[15] = _source[36] / _source[37];
    ans[16] = _source[38] / _source[39];
    ans[17] = _source[40] * _source[41] / _source[42];
    ans[18] = _source[43] + _source[44] + _source[45];
    ans[19] =  1.0        - _source[46] / _source[47];
    ans[20] = _source[48] + _source[49] + _source[50];
    ans[21] =(_source[51] + _source[52] + _source[53])/ _source[54];
    ans[22] = _source[55] + _source[56] + _source[57] + _source[58];
    ans[23] = _source[59] * _source[60] * _source[61];
    ans[24] = _source[62] / _source[63];
    ans[25] = _source[64] / _source[65];
    ans[26] = _source[66] + _source[67] + _source[68] + _source[69];
    ans[27] = _source[70] + _source[71] + _source[72];
    ans[28] = 1140 * _source[74] / _source[73] +
             (_source[75] * _source[76] * _source[77]) / _source[78]; // train + auto
    ans[29] = _source[79] / _source[80];
    ans[30] = _source[81] / _source[82];
    ans[31] = _source[83] / _source[84];
    ans[32] = _source[85] + _source[86] + _source[87] + _source[88];
    ans[33] = _source[89] + _source[90] + _source[91];
    ans[34] = _source[92] / _source[93] + (_source[94] * _source[95]) / _source[96];//
    ans[35] = _source[97] / _source[98];
//    for(int i = 36; i <= 57; i++)
//    {
//       ans[i] = _source[i + 63];
//    }
    return ans.toList();
}
