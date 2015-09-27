#include "projectsourcedata.h"
#include "ui_projectsourcedata.h"
#include "treepropertywidget.h"

ProjectSourceData::ProjectSourceData(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProjectSourceData),
    m_tree(new TreePropertyWidget("constants.txt"))
{
    m_ui->setupUi(this);
    m_ui->verticalLayout->addWidget(m_tree);

    connect(m_ui->calculate, SIGNAL(clicked()), SLOT(callCalculation()));

}

ProjectSourceData::~ProjectSourceData()
{
    delete m_ui;
}

void ProjectSourceData::callCalculation()
{
    TreePropertyWidget* calculation = new TreePropertyWidget("factors.txt");
//    calculation->showMaximized();
    calculation->show();
}
