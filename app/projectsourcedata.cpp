#include <QDebug>
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
    m_tree->setCurrentJudge(QString("constants"), false);

    connect(m_ui->calculate, SIGNAL(clicked()), SLOT(callCalculation()));

}

ProjectSourceData::~ProjectSourceData()
{
//    m_tree->valuesToFile(m_tree->currentJudge(), QString("sourceData.txt"));
    delete m_ui;
}

void ProjectSourceData::callCalculation()
{
    QList<double> vals = calculateProject(m_tree->values());
    qDebug() << vals;

    TreePropertyWidget* calculation = new TreePropertyWidget("factors.txt");
    calculation->setValues("Project1", vals);
    calculation->setCurrentJudge("Project1", false);
//    calculation->showMaximized();
    calculation->show();
}

QList<double> ProjectSourceData::calculateProject(QList<double> _source) const
{
    QList<double> ans = _source;


    return ans;
}
