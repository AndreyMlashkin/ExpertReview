#include "modechooser.h"
#include "ui_modechooser.h"

#include "propertytreeviewer.h"
#include "projectsourcedata.h"

ModeChooser::ModeChooser(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ModeChooser)
{
    m_ui->setupUi(this);
    connect(m_ui->metodicJudges, SIGNAL(clicked()), SLOT(callMetodicJudges()));
    connect(m_ui->calculation,   SIGNAL(clicked()), SLOT(callCalculation()));
}

ModeChooser::~ModeChooser()
{
    delete m_ui;
}

void ModeChooser::callMetodicJudges()
{
    PropertyTreeViewer* metodicJudges = new PropertyTreeViewer();
    metodicJudges->show();
}

void ModeChooser::callCalculation()
{
    ProjectSourceData* data = new ProjectSourceData();
    data->show();
}
