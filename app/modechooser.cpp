#include "modechooser.h"
#include "ui_modechooser.h"

#include "propertytreeviewer.h"
#include "projectsourcedata.h"

ModeChooser::ModeChooser(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ModeChooser),
    m_viewer(NULL),
    m_sourceData(NULL)
{
    m_ui->setupUi(this);
    connect(m_ui->metodicJudges, SIGNAL(clicked()), SLOT(callMetodicJudges()));
    connect(m_ui->sectionJudges, SIGNAL(clicked()), SLOT(callSectionJudges()));
    connect(m_ui->sourceData,    SIGNAL(clicked()), SLOT(callSourceData()));
    connect(m_ui->calculation,   SIGNAL(clicked()), SLOT(callCalculation()));
}

ModeChooser::~ModeChooser()
{
    delete m_viewer;
    delete m_sourceData;
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
    PropertyTreeViewer* sourceData = new PropertyTreeViewer("constants");
    sourceData->setDefaultTabName("Проект");
    sourceData->show();
}

void ModeChooser::callCalculation()
{
    ProjectSourceData* data = new ProjectSourceData();
    data->show();
}
