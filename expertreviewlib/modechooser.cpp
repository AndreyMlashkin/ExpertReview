#include <QDebug>
#include "modechooser.h"
#include "ui_modechooser.h"

#include <QPointer>

#include "serialization/nodesinfo/leftsidesconstants.h"
#include "projectapi.h"
#include "serialization/nodesinfo/treerightsidevalues.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"
#include "serialization/projectsloader.h"
#include "propertytreeviewer.h"
#include "projectcalculation.h"
#include "finalcalculationdialog.h"
#include "rightsidediagrammview.h"
#include "helpapi.h"

struct ModeChooser::ModeChooserPrivate
{
    ModeChooserPrivate()
        : metodicJudges(nullptr),
          sectionJudges(nullptr),
          RangedConstantsJudges(nullptr),
          sourceData(nullptr),
          calculation(nullptr),
          sectionCalculation(nullptr),
          rightSideDiagrammView(nullptr)
    {}

    ~ModeChooserPrivate()
    {
        delete metodicJudges;
        delete sectionJudges;
        delete RangedConstantsJudges;
        delete sourceData;
        delete calculation;
        delete sectionCalculation;
        delete rightSideDiagrammView;
    }

    QPointer <PropertyTreeViewer> metodicJudges;
    QPointer <PropertyTreeViewer> sectionJudges;
    QPointer <PropertyTreeViewer> RangedConstantsJudges;
    QPointer <PropertyTreeViewer> sourceData;
    QPointer <PropertyTreeViewer> calculation;
    QPointer <PropertyTreeViewer> sectionCalculation;
    QPointer <RightSideDiagrammView> rightSideDiagrammView;
};

ModeChooser::ModeChooser(const ProjectsLoaderPtr& _loader, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ModeChooser),
    m_loader(_loader),
    p(new ModeChooserPrivate)
{
    m_ui->setupUi(this);

    connect(m_ui->metodicJudges,        SIGNAL(clicked()), SLOT(callMetodicJudges()));
    connect(m_ui->sectionJudges,        SIGNAL(clicked()), SLOT(callSectionJudges()));
    connect(m_ui->RangedConstantsJudges,  SIGNAL(clicked()), SLOT(callRangedConstantsJudges()));
    connect(m_ui->sourceData,           SIGNAL(clicked()), SLOT(callSourceData()));
    connect(m_ui->calculation,          SIGNAL(clicked()), SLOT(callCalculation()));
    connect(m_ui->sectionsCalculation,  SIGNAL(clicked()), SLOT(callSectionCalculation()));
    connect(m_ui->help,                 SIGNAL(clicked()), SLOT(callHelp()));
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
    p->metodicJudges = new PropertyTreeViewer(m_loader, serializeConstants::metodicJudges,
    PropertyTreeViewer::All ^ PropertyTreeViewer::FinalCastTab);

    p->metodicJudges->show();
}

void ModeChooser::callSectionJudges()
{
    delete p->sectionJudges;
    p->sectionJudges.clear();
    p->sectionJudges = new PropertyTreeViewer(m_loader, serializeConstants::sections);
    p->sectionJudges->show();
}

void ModeChooser::callRangedConstantsJudges()
{
    // should be for every project
    delete p->RangedConstantsJudges;
    p->RangedConstantsJudges.clear();
    p->RangedConstantsJudges = new PropertyTreeViewer(m_loader, serializeConstants::RangedConstantsJudges,
        PropertyTreeViewer::All ^ PropertyTreeViewer::FinalCastTab);
    p->RangedConstantsJudges->show();
}

void ModeChooser::callSourceData()
{
    delete p->sourceData;
    p->sourceData = new PropertyTreeViewer(m_loader, serializeConstants::constants, PropertyTreeViewer::Minimal);
    p->sourceData->setPrecision(4);
    p->sourceData->show();
}

void ModeChooser::callSectionCalculation()
{
    updateResult();
    ProjectCalculator::updateSectionCalculation(m_loader);

    delete p->sectionCalculation;
    p->sectionCalculation = new PropertyTreeViewer(m_loader, serializeConstants::sectionsResult, PropertyTreeViewer::Minimal);
    p->sectionCalculation->setPrecision(6);
    p->sectionCalculation->show();

    // TODO move to project calculation
    TreeLeftSideInfo* sectionsResult = m_loader->getLeftSideInfo(serializeConstants::sectionsResult);
    Q_ASSERT(sectionsResult);
    auto existingRSides = sectionsResult->getRightSides();
    Q_ASSERT(sectionsResult->getRightSides().size() <= 2); // Can also add a check for names

    TreeRightSideValues* section1Result =
            m_loader->getOrCreateRightSide(serializeConstants::sectionsResult, "sectionsResult0", false);
    TreeRightSideValues* section2Result =
            m_loader->getOrCreateRightSide(serializeConstants::sectionsResult, "sectionsResult1", false);
    Q_ASSERT(section1Result && section2Result);
    Q_ASSERT(sectionsResult->getRightSides().size() == 2);

    QList<double> firstResult  = section1Result->values().values();
    QList<double> secondResult = section2Result->values().values();

    double firstFinalCriterium  = ProjectCalculator::sumAll(firstResult);
    double secondFinalCriterium = ProjectCalculator::sumAll(secondResult);

    FinalCalculationDialog dialog(m_loader,
                                  firstFinalCriterium,
                                  secondFinalCriterium,
                                  serializeConstants::sectionsResult, this);
    dialog.exec();

    delete p->rightSideDiagrammView.data();
    p->rightSideDiagrammView = new RightSideDiagrammView(sectionsResult);
    p->rightSideDiagrammView->show();
}

void ModeChooser::callCalculation()
{
    updateResult();

    delete p->calculation;
    p->calculation = new PropertyTreeViewer(m_loader, serializeConstants::result, PropertyTreeViewer::Minimal);
    p->calculation->setPrecision(6);
    p->calculation->show();
    // TODO move to project calculation
    TreeLeftSideInfo* leftSide = m_loader->getLeftSideInfo(serializeConstants::result);
    Q_ASSERT(leftSide);

    TreeRightSideValues* proj1Result =
            m_loader->getOrCreateRightSide(serializeConstants::result, "result0", false);
    TreeRightSideValues* proj2Result =
            m_loader->getOrCreateRightSide(serializeConstants::result, "result1", false);
    Q_ASSERT(proj1Result && proj2Result);

    QList<double> firstResult  = proj1Result->values().values();
    QList<double> secondResult = proj2Result->values().values();

    double firstFinalCriterium  = ProjectCalculator::sumAll(firstResult);
    double secondFinalCriterium = ProjectCalculator::sumAll(secondResult);

    FinalCalculationDialog dialog(m_loader,
                                  firstFinalCriterium,
                                  secondFinalCriterium,
                                  serializeConstants::result, this);
    dialog.exec();
}

void ModeChooser::callHelp()
{
    QString text = "1. Выберите в этом окне тип экспертов для работы.\n"
                   "2. В следующем окне, заполните данные, используя "
                   "импорт подготовленных экспертами файлов или добавьте "
                   "новых экспертов и заполните данные вручную\n"
                   "3. Расчётные вкладки будут обновлены автоматически";
    ::callHelp(text);
}

void ModeChooser::updateResult()
{
    ProjectCalculator calc(m_loader);
    calc.calculate();
}

void ModeChooser::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    qApp->closeAllWindows();
}
