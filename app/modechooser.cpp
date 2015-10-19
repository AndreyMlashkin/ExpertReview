#include "modechooser.h"
#include "ui_modechooser.h"

#include "projectapi.h"
#include "nodesinfo/treerightsidevalues.h"
#include "nodesinfo/treeleftsideinfofactory.h"
#include "nodesinfo/treeleftsideinfo.h"
#include "propertytreeviewer.h"
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

inline TreeRightSideValues* getAverage(TreeLeftSideInfoFactory* _factory, const QString& _name)
{
    TreeLeftSideInfo* leftSide = _factory->getLeftSideInfo(_name);
    TreeRightSideValues* rightAverage = leftSide->createRightSide();
    rightAverage->readValues(leftSide->savedAverageRightSideId());
    return rightAverage;
}

void ModeChooser::callCalculation()
{
    TreeLeftSideInfoFactory* factory = new TreeLeftSideInfoFactory();
    TreeRightSideValues* methodicJudgesAverage = getAverage(factory, "metodicJudges");
    TreeRightSideValues* sectionsAverage = getAverage(factory, "sections");

    TreeLeftSideInfo* constants = factory->getLeftSideInfo("constants");
    TreeLeftSideInfo* result    = factory->getLeftSideInfo("result");

    ProjectCalculator calc(methodicJudgesAverage, sectionsAverage);
    calc.calculate(constants, result);

    PropertyTreeViewer* calculation = new PropertyTreeViewer("result", PropertyTreeViewer::Minimal);
    calculation->setDefaultTabName("Проект");
    calculation->setWindowTitle("Расчёт");
    calculation->setPrecision(6);
    calculation->show();

    TreeLeftSideInfo* leftSide = factory->getLeftSideInfo("result");
    TreeRightSideValues* proj1Result = leftSide->createRightSide();
    TreeRightSideValues* proj2Result = leftSide->createRightSide();
    proj1Result->readValues(leftSide->savedRightSidesIds().at(0));
    proj2Result->readValues(leftSide->savedRightSidesIds().at(1));

//    TreeRightSideValues* proj1Result = factory->getRightSideValues("result0");
//    TreeRightSideValues* proj2Result = factory->getRightSideValues("result1");

    QList<double> firstResult  = proj1Result->values().values();
    QList<double> secondResult = proj2Result->values().values();

    double firstFinalCriterium  = calculateFinalCriterium(firstResult);
    double secondFinalCriterium = calculateFinalCriterium(secondResult);

    FinalCalculationDialog dialog(firstFinalCriterium, secondFinalCriterium, this);
    dialog.exec();
}
