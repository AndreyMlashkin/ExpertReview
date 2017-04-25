#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QInputDialog>

#include "projectapi.h"
#include "projectchooser.h"
#include "ui_projectchooser.h"
#include "serialization/projectsloader.h"
#include "helpapi.h"

ProjectChooser::ProjectChooser(const ProjectsLoaderPtr& _loader, bool _inputExpertName, const QString &_projectFileName, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProjectChooser),
    m_loader(_loader),
    m_newClicked(false),
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
    connect(m_ui->addNewProject, SIGNAL(clicked(bool)), SLOT(addNewProject()));
}

ProjectChooser::~ProjectChooser()
{
    delete m_ui;
}

void ProjectChooser::updateProjectList()
{
    clearGui();
    m_foundProjects.clear();

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

        QString textToDisplay = project.isDir()? project.baseName() : project.dir().dirName();
        projectButton->setText(textToDisplay);
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
    Q_ASSERT(button);

    if(m_newClicked)
    {
        setupNewProject(button->text());
        updateProjectList();
        m_newClicked = false;
        return;
    }

    if(!button || !m_projectsBind.contains(button))
    {
        qDebug() << Q_FUNC_INFO << " something went wrong";
        return;
    }

    const auto& project = m_projectsBind[button];
    qDebug() << "choosen " << project.absoluteFilePath();

    bool loaded = m_loader->load(project);
    if(!loaded)
    {
        qCritical() << "Error reading file";
        QGuiApplication::exit(-1);
    }

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

QFileInfoList ProjectChooser::getProjectPatterns()
{
    QDir projectDirecotry = QDir::current();
    const QString patternsDirName(fileNames::patternsDir);
    QFileInfoList projectPatterns;
    if(projectDirecotry.cd(patternsDirName))
    {
        projectPatterns = projectDirecotry.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    }
    return projectPatterns;
}

void ProjectChooser::setupNewProject(const QString& patternName)
{
    QString newProjectName = QInputDialog::getText(this, "Новый проект", "Введите название проекта");
    QDir::current().mkdir(newProjectName);

    QDir newProjectPath = QDir::current();
    newProjectPath.cd(newProjectName);

    QDir choosenPattern = projectPatternsDir();
    choosenPattern.cd(patternName);

    copyDir(choosenPattern, newProjectPath);
}

QDir ProjectChooser::projectPatternsDir()
{
    QDir current = QDir::current();
    current.cd("project_patterns");
    return current;
}

bool ProjectChooser::copyDir(const QDir &src, const QDir &dest)
{
    if(!src.isReadable()) return false;
    QFileInfoList entries = src.entryInfoList();
    for(QList<QFileInfo>::iterator it = entries.begin(); it!=entries.end();++it)
    {
        QFileInfo &finfo = *it;
        if(finfo.fileName()=="." || finfo.fileName()=="..") continue;
        //if(finfo.isDir()){ copyDir(finfo.filePath()); continue; }
        //if(finfo.isSymLink()) { /* do something here */ continue; }
        if(finfo.isFile() && finfo.isReadable())
        {
            QFile file(finfo.filePath());
            file.copy(dest.absoluteFilePath(finfo.fileName()));
        } else return false;
    }
    return true;
}

void ProjectChooser::helpCalled()
{
    QString text = "1. Введите имя и фамилию через пробел.\n"
                   "2. Выберите один из доступных проектов";
    callHelp(text);
}

void ProjectChooser::addNewProject()
{
    m_newClicked = true;
    QFileInfoList projectsPatterns = getProjectPatterns();
    if(projectsPatterns.isEmpty())
        return;

    m_foundProjects = projectsPatterns;
    updateProjectListGui();
    hideAddButton();
}
