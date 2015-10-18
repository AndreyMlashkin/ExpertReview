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
    metodicJudges->setWindowTitle("Эксперты методики");
    metodicJudges->show();
}

void ModeChooser::callSectionJudges()
{
    PropertyTreeViewer* sectionJudges = new PropertyTreeViewer("sections");
    sectionJudges->setDefaultTabName("Эксперт");
    sectionJudges->setWindowTitle("Эксперты разделов");
    sectionJudges->show();
}

void ModeChooser::callSourceData()
{
    PropertyTreeViewer* sourceData = new PropertyTreeViewer("constants", PropertyTreeViewer::SaveRegularOnExit);
    sourceData->setDefaultTabName("Проект");
    sourceData->setWindowTitle("Исходные данные проектов");
    sourceData->setPrecision(4);
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
    TreeInfoFactory* factory = new TreeInfoFactory();
    ProjectCalculator calc("metodicJudges_average", "sections_average", factory);
    calc.calculate("constants0", "constants1", "result0", "result1");


    PropertyTreeViewer* calculation = new PropertyTreeViewer("result", PropertyTreeViewer::Minimal);
    calculation->setDefaultTabName("Проект");
    calculation->setWindowTitle("Расчёт");
    calculation->setPrecision(6);
    calculation->show();


    TreeRightSideValues* proj1Result = factory->getRightSideValues("result0");
    TreeRightSideValues* proj2Result = factory->getRightSideValues("result1");
    QList<double> firstResult  = toDoubleList(proj1Result->values());
    QList<double> secondResult = toDoubleList(proj2Result->values());

    double firstFinalCriterium  = calculateFinalCriterium(firstResult);
    double secondFinalCriterium = calculateFinalCriterium(secondResult);

    FinalCalculationDialog dialog(firstFinalCriterium, secondFinalCriterium, this);
    dialog.exec();
}
