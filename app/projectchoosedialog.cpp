#include <QDebug>
#include <QDir>

#include "projectchoosedialog.h"
#include "ui_projectchoosedialog.h"
#include "serialization/projectsloader.h"

ProjectChooseDialog::ProjectChooseDialog(const std::shared_ptr<ProjectsLoader> &loader, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProjectChooseDialog)
{
    m_ui->setupUi(this);
    updateProjectList();

    connect(this, &ProjectChooseDialog::projectChoosen, loader.get(), &ProjectsLoader::load);
}

ProjectChooseDialog::~ProjectChooseDialog()
{
    delete m_ui;
}

void ProjectChooseDialog::updateProjectList()
{
    QDir projectDirecotry = QDir::current();
    auto directories = projectDirecotry.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(const auto& dirName : directories)
    {
        QDir subDir(dirName.absoluteFilePath());
        auto files = subDir.entryInfoList(QDir::Files);
        for(const auto& file : files)
        {
            if(file.baseName() == "project")
            {
                m_foundProjects << QFileInfo(file.absoluteFilePath());
                break;
            }
        }
    }
    updateProjectListGui();
}

void ProjectChooseDialog::updateProjectListGui()
{
    clearGui();

    QPushButton* example = m_ui->addNewProject;

    for(const QFileInfo& project : m_foundProjects)
    {
        QPushButton* projectButton = new QPushButton();
        projectButton->setFont(example->font());
        projectButton->setText(project.dir().dirName());
        projectButton->setSizePolicy(example->sizePolicy());
        m_ui->projectButtonslayout->addWidget(projectButton);
        m_projectsBind[projectButton] = project;

        connect(projectButton, &QPushButton::clicked, this, &ProjectChooseDialog::projectClicked);
    }
}

void ProjectChooseDialog::projectClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(!button || !m_projectsBind.contains(button))
    {
        qDebug() << Q_FUNC_INFO << " something went wrong";
        return;
    }

    const auto& project = m_projectsBind[button];
    qDebug() << "choosen " << project.absoluteFilePath();
    emit projectChoosen(project);
}

void ProjectChooseDialog::clearGui()
{
    for(QPushButton* button : m_projectsBind.keys())
    {
        button->deleteLater();
    }
    m_projectsBind.clear();
}

