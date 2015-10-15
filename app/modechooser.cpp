#include <QFileInfo>

#include "modechooser.h"
#include "ui_modechooser.h"

#include "projectapi.h"
#include "nodesinfo/treerightsidevalues.h"
#include "nodesinfo/treeinfofactory.h"
#include "propertytreeviewer.h"
#include "projectsourcedata.h"
#include "projectcalculation.h"
#include "finalcalculationdialog.h"

ModeChooser::ModeChooser(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ModeChooser)
{
    m_ui->setupUi(this);
    connect(m_ui->metodicJudges, SIGNAL(clicked()), SLOT(callMetodicJudges()));
    connect(m_ui->sectionJudges, SIGNAL(clicked()), SLOT(callSectionJudges()));
    connect(m_ui->sourceData,    SIGNAL(clicked()), SLOT(callSourceData()));
    connect(m_ui->calculation,   SIGNAL(clicked()), SLOT(callCalculation()));
}

ModeChooser::~ModeChooser()
{
    delete m_ui;
}

void ModeChooser::callMetodicJudges()
{
    PropertyTreeViewer* metodicJudges = new PropertyTreeViewer("metodicJudges");
    metodicJudges->setDefaultTabName("Эксперт");
    metodicJudges->show();
}

void ModeChooser::callSectionJudges()
{
    PropertyTreeViewer* sectionJudges = new PropertyTreeViewer("sections");
    sectionJudges->setDefaultTabName("Эксперт");
    sectionJudges->show();
}

void ModeChooser::callSourceData()
{
    PropertyTreeViewer* sourceData = new PropertyTreeViewer("constants", PropertyTreeViewer::SaveRegularOnExit);
    sourceData->setDefaultTabName("Проект");
    sourceData->show();
}

inline double calculateFinalCriterium(QList<double>& _projectKoeffs)
{
    double ans = 0;
    foreach(double val, _projectKoeffs)
        ans += val;
    return ans;
}

void ModeChooser::callCalculation()
{
//    int count = 0;
//    QStringList projectsNames;
//    forever
//    {
//        QString path = "constants" + QString::number(count++);
//        QFileInfo info(path);
//        if(info.exists())
//            projectsNames << path;
//        else
//            break;
//    }

    ProjectCalculator::calculate("metodicJudges_average",
                                 "sections_average",
                                 "constants0",
                                 "constants1",
                                 "result");

    PropertyTreeViewer* calculation = new PropertyTreeViewer("result", PropertyTreeViewer::Minimal);
    calculation->setDefaultTabName("Проект");
    calculation->show();

    TreeInfoFactory* factory = new TreeInfoFactory();
    TreeRightSideValues* proj1Result = factory->getRightSideValues("result0");
    TreeRightSideValues* proj2Result = factory->getRightSideValues("result1");

    QList<double> firstResult  = toDoubleList(proj1Result->values());
    QList<double> secondResult = toDoubleList(proj2Result->values());

    double firstFinalCriterium  = calculateFinalCriterium(firstResult);
    double secondFinalCriterium = calculateFinalCriterium(secondResult);

    FinalCalculationDialog dialog(firstFinalCriterium, secondFinalCriterium, this);
    dialog.exec();
}
