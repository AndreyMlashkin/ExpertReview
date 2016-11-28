#include <QDebug>
#include <QDir>

#include "projectchooser.h"
#include "ui_projectchooser.h"
#include "serialization/projectsloader.h"
#include "helpapi.h"

ProjectChooser::ProjectChooser(const ProjectsLoaderPtr& _loader, bool _inputExpertName, const QString &_projectFileName, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProjectChooser),
    m_loader(_loader),
    m_projectFileName(_projectFileName),
    m_initialExpertNameTextSet(true)
{
    m_ui->setupUi(this);
    updateProjectList();

    if(_inputExpertName)
        inputExpertName();
    else
    {
        m_ui->expertNameInput->hide();
    }
    connect(m_ui->help, SIGNAL(clicked(bool)), SIGNAL(helpClicked()));
    connect(m_ui->help, SIGNAL(clicked(bool)), SLOT(helpCalled()));
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
            if(file.baseName() == m_projectFileName)
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

void ProjectChooser::hideAddButton()
{
    m_ui->addNewProject->hide();
}

void ProjectChooser::inputExpertName()
{
    m_ui->expertNameInput->installEventFilter(this);
    enableAllProjects(false);
}

QString ProjectChooser::getExpertName() const
{
    return m_ui->expertNameInput->text();
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

    m_loader->load(project);

    emit projectChoosen();
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

void ProjectChooser::enableAllProjects(bool enable)
{
    for(QPushButton* project : m_projectsBind.keys())
        project->setEnabled(enable);
}

bool ProjectChooser::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_ui->expertNameInput &&
            (event->type() == QEvent::KeyPress ||
             event->type() == QEvent::MouseButtonPress))
    {
        if(m_initialExpertNameTextSet)
        {
            // strikes once
            m_ui->expertNameInput->setText("");
            QFont font = m_ui->expertNameInput->font();
            font.setItalic(false);
            m_ui->expertNameInput->setFont(font);
            m_initialExpertNameTextSet = false;
        }
        else if(m_ui->expertNameInput->text().size() > 6 &&
                m_ui->expertNameInput->text().contains(" "))
        {
            enableAllProjects(true);
        }
        else
        {
            enableAllProjects(false);
        }
    }
    return false;
}

void ProjectChooser::helpCalled()
{
    QString text = "1. Введите имя и фамилию через пробел.\n"
                   "2. Выберите один из доступных проектов";
    callHelp(text);
}
