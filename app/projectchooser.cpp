#include <QDebug>
#include <QDir>

#include "projectchooser.h"
#include "ui_projectchooser.h"
#include "serialization/projectsloader.h"

ProjectChooser::ProjectChooser(const ProjectsLoaderPtr& _loader, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProjectChooser)
{
    m_ui->setupUi(this);
    updateProjectList();

    connect(this, &ProjectChooser::projectChoosen, _loader.data(), &ProjectsLoader::load);
}

ProjectChooser::~ProjectChooser()
{
    delete m_ui;
}

void ProjectChooser::updateProjectList()
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

void ProjectChooser::updateProjectListGui()
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

        connect(projectButton, &QPushButton::clicked, this, &ProjectChooser::projectClicked);
    }
}

void ProjectChooser::projectClicked()
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
    hide();
}

void ProjectChooser::clearGui()
{
    for(QPushButton* button : m_projectsBind.keys())
    {
        button->deleteLater();
    }
    m_projectsBind.clear();
}

