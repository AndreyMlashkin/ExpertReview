#include "modechooser.h"
#include "ui_modechooser.h"

#include <QPointer>
#include <QFile>

#include "projectapi.h"
#include "serialization/nodesinfo/treerightsidevalues.h"
#include "serialization/nodesinfo/treeleftsideinfofactory.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"
#include "serialization/projectsloader.h"
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
        delete calculation;
    }

    QPointer <PropertyTreeViewer> metodicJudges;
    QPointer <PropertyTreeViewer> sectionJudges;
    QPointer <PropertyTreeViewer> sourceData;
    QPointer <PropertyTreeViewer> calculation;
    QPointer <PropertyTreeViewer> sectionCalculation;
};

ModeChooser::ModeChooser(const ProjectsLoaderPtr& _loader, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ModeChooser),
    m_loader(_loader),
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
    p->metodicJudges = new PropertyTreeViewer(m_loader, "metodicJudges");
    p->metodicJudges->show();
}

void ModeChooser::callSectionJudges()
{
    delete p->sectionJudges;
    p->sectionJudges.clear();
    p->sectionJudges = new PropertyTreeViewer(m_loader, "sections");
    p->sectionJudges->show();
}

void ModeChooser::callSourceData()
{
    delete p->sourceData;
    p->sourceData = new PropertyTreeViewer(m_loader, "constants", PropertyTreeViewer::SaveRegularOnExit);
    p->sourceData->setPrecision(4);
    p->sourceData->show();
}

// TODO move to calculator
inline double calculateFinalCriterium(QList<double>& _projectKoeffs)
{
    double ans = 0;
    foreach(double val, _projectKoeffs)
        ans += val;
    return ans;
}


void ModeChooser::callSectionCalculation()
{
    delete p->sectionCalculation;
    p->sectionCalculation = new PropertyTreeViewer(m_loader, "sectionsResult", PropertyTreeViewer::Minimal);
    p->sectionCalculation->setPrecision(6);
    p->sectionCalculation->show();

    ProjectCalculator::updateSectionCalculation(m_loader);

    TreeLeftSideInfo* sectionsResult = m_loader->getLeftSideInfo("sectionsResult");
    Q_ASSERT(sectionsResult);
    Q_ASSERT(sectionsResult->getRightSides().count() == 2);


    auto sectionsRSides = sectionsResult->getRightSides();
    QList<double> firstResult  = sectionsRSides.at(0)->values().values();
    QList<double> secondResult = sectionsRSides.at(1)->values().values();

    double firstFinalCriterium  = calculateFinalCriterium(firstResult);
    double secondFinalCriterium = calculateFinalCriterium(secondResult);

    FinalCalculationDialog dialog(m_loader,
                                  firstFinalCriterium,
                                  secondFinalCriterium,
                                  "sectionsResult", this);
    dialog.exec();
}

void ModeChooser::callCalculation()
{
    updateResult();

    delete p->calculation;
    p->calculation = new PropertyTreeViewer(m_loader, "result", PropertyTreeViewer::Minimal);
    p->calculation->setPrecision(6);
    p->calculation->show();

    TreeLeftSideInfo* leftSide = m_loader->getLeftSideInfo("result");
    Q_ASSERT(leftSide);

    TreeRightSideValues* proj1Result =
            m_loader->getOrCreateRightSide("result", "result0", true);
    TreeRightSideValues* proj2Result =
            m_loader->getOrCreateRightSide("result", "result1", true);
    Q_ASSERT(proj1Result && proj2Result);

    QList<double> firstResult  = proj1Result->values().values();
    QList<double> secondResult = proj2Result->values().values();

    double firstFinalCriterium  = calculateFinalCriterium(firstResult);
    double secondFinalCriterium = calculateFinalCriterium(secondResult);

    FinalCalculationDialog dialog(m_loader,
                                  firstFinalCriterium,
                                  secondFinalCriterium,
                                  "result", this);
    dialog.exec();
}

void ModeChooser::updateResult()
{
    TreeRightSideValues* methodicJudgesAverage =
            ProjectCalculator::getAverageRightSide(m_loader, "metodicJudges");
    TreeRightSideValues* sectionsAverage =
            ProjectCalculator::getAverageRightSide(m_loader, "sections");

    TreeLeftSideInfo* constants = m_loader->getLeftSideInfo("constants");
    TreeLeftSideInfo* result    = m_loader->getLeftSideInfo("result");

    TreeLeftSideInfo* methodicJudges = m_loader->getLeftSideInfo("metodicJudges");
    ProjectCalculator calc(methodicJudges, methodicJudgesAverage, sectionsAverage);
    calc.calculate(constants, result);

    // section calculation:
    TreeLeftSideInfo* sectionsResult    = m_loader->getLeftSideInfo("sectionsResult");
//    TreeLeftSideInfo* methodicJudges    = m_loader->getLeftSideInfo("metodicJudges");
    TreeLeftSideInfo* calculatedFactors = m_loader->getLeftSideInfo("result");
    calc.calculateSections(calculatedFactors, sectionsResult);
}

void ModeChooser::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    qApp->closeAllWindows();
}
