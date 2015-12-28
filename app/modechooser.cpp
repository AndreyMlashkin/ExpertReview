#include "modechooser.h"
#include "ui_modechooser.h"

#include <QPointer>
#include <QFile>

#include "projectapi.h"
#include "nodesinfo/treerightsidevalues.h"
#include "nodesinfo/treeleftsideinfofactory.h"
#include "nodesinfo/treeleftsideinfo.h"
#include "propertytreeviewer.h"
#include "projectcalculation.h"
#include "finalcalculationdialog.h"

struct ModeChooser::ModeChooserPrivate
{
    ModeChooserPrivate()
        : metodicJudges(NULL),
          sectionJudges(NULL),
          sourceData(NULL)
    {}

    ~ModeChooserPrivate()
    {
        delete metodicJudges;
        delete sectionJudges;
        delete sourceData;
    }

    QPointer <PropertyTreeViewer> metodicJudges;
    QPointer <PropertyTreeViewer> sectionJudges;
    QPointer <PropertyTreeViewer> sourceData;
    QPointer <PropertyTreeViewer> calculation;
};

ModeChooser::ModeChooser(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ModeChooser),
    p(new ModeChooserPrivate)
{
    m_ui->setupUi(this);
    connect(m_ui->metodicJudges, SIGNAL(clicked()), SLOT(callMetodicJudges()));
    connect(m_ui->sectionJudges, SIGNAL(clicked()), SLOT(callSectionJudges()));
    connect(m_ui->sourceData,    SIGNAL(clicked()), SLOT(callSourceData()));
    connect(m_ui->calculation,   SIGNAL(clicked()), SLOT(callCalculation()));
    connect(m_ui->sectionsCalculation,   SIGNAL(clicked()), SLOT(callSectionCalculation()));
}

ModeChooser::~ModeChooser()
{
    delete m_ui;
    delete p;
}

void ModeChooser::callMetodicJudges()
{
    delete p->metodicJudges;
    p->metodicJudges.clear();
    p->metodicJudges = new PropertyTreeViewer("metodicJudges");
    p->metodicJudges->setDefaultTabName("Эксперт");
    p->metodicJudges->setWindowTitle("Эксперты методики");
    p->metodicJudges->show();
}

void ModeChooser::callSectionJudges()
{
    delete p->sectionJudges;
    p->sectionJudges.clear();
    p->sectionJudges = new PropertyTreeViewer("sections");
    p->sectionJudges->setDefaultTabName("Эксперт");
    p->sectionJudges->setWindowTitle("Эксперты разделов");
    p->sectionJudges->show();
}

void ModeChooser::callSourceData()
{
    delete p->sourceData;
    p->sourceData = new PropertyTreeViewer("constants", PropertyTreeViewer::SaveRegularOnExit);
    p->sourceData->setDefaultTabName("Проект");
    p->sourceData->setWindowTitle("Исходные данные проектов");
    p->sourceData->setPrecision(4);
    p->sourceData->show();
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
    rightAverage->readValues(leftSide->savedAverageRightSideTreeName());
    return rightAverage;
}
void createResulSectionstLeftSideFile()
{
    QFile::copy("sections", "sectionsResult");
}

void ModeChooser::callSectionCalculation()
{
    createResulSectionstLeftSideFile();
    updateResult();

    TreeLeftSideInfoFactory* factory = new TreeLeftSideInfoFactory();
    TreeRightSideValues* methodicJudgesAverage = getAverage(factory, "metodicJudges");
    TreeRightSideValues* sectionsAverage = getAverage(factory, "sections");

    TreeLeftSideInfo* sectionsResult    = factory->getLeftSideInfo("sectionsResult");
    TreeLeftSideInfo* methodicJudges    = factory->getLeftSideInfo("metodicJudges");
    TreeLeftSideInfo* calculatedFactors = factory->getLeftSideInfo("result");
    ProjectCalculator calc(methodicJudges, methodicJudgesAverage, sectionsAverage);
    calc.calculateSections(calculatedFactors, sectionsResult);

    PropertyTreeViewer* sectionResult = new PropertyTreeViewer("sectionsResult", PropertyTreeViewer::SaveRegularOnExit);
    sectionResult->setDefaultTabName("Проект");
    sectionResult->setWindowTitle("Расчёт разделов");
    sectionResult->setPrecision(6);
    sectionResult->show();
}

void createResultLeftSideFile()
{
    QFile::copy("metodicJudges", "result");
}

void ModeChooser::callCalculation()
{
    updateResult();

    delete p->calculation;
    p->calculation = new PropertyTreeViewer("result", PropertyTreeViewer::Minimal);
    p->calculation->setDefaultTabName("Проект");
    p->calculation->setWindowTitle("Расчёт");
    p->calculation->setPrecision(6);
    p->calculation->show();

    TreeLeftSideInfoFactory* factory = new TreeLeftSideInfoFactory();
    TreeLeftSideInfo* leftSide = factory->getLeftSideInfo("result");
    TreeRightSideValues* proj1Result = leftSide->createRightSide();
    TreeRightSideValues* proj2Result = leftSide->createRightSide();
    proj1Result->readValues(leftSide->savedRightSidesIds().at(0));
    proj2Result->readValues(leftSide->savedRightSidesIds().at(1));

    QList<double> firstResult  = proj1Result->values().values();
    QList<double> secondResult = proj2Result->values().values();

    double firstFinalCriterium  = calculateFinalCriterium(firstResult);
    double secondFinalCriterium = calculateFinalCriterium(secondResult);

    FinalCalculationDialog dialog(firstFinalCriterium, secondFinalCriterium, this);
    dialog.exec();

    delete factory;
}

void ModeChooser::updateResult()
{
    createResultLeftSideFile();

    TreeLeftSideInfoFactory* factory = new TreeLeftSideInfoFactory();
    TreeRightSideValues* methodicJudgesAverage = getAverage(factory, "metodicJudges");
    TreeRightSideValues* sectionsAverage = getAverage(factory, "sections");

    TreeLeftSideInfo* constants = factory->getLeftSideInfo("constants");
    TreeLeftSideInfo* result    = factory->getLeftSideInfo("result");

    TreeLeftSideInfo* methodicJudges = factory->getLeftSideInfo("metodicJudges");
    ProjectCalculator calc(methodicJudges, methodicJudgesAverage, sectionsAverage);
    calc.calculate(constants, result);

    delete factory;
}
