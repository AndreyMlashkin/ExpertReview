#ifndef PROJECTCHOOSER_H
#define PROJECTCHOOSER_H

#include <QWidget>
#include <QMap>
#include <QFileInfo>
#include <QFileInfo>

#include "serialization/projectsloader.h"

namespace Ui {
class ProjectChooser;
}

class QPushButton;
class ProjectsLoader;

class ProjectChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectChooser(const ProjectsLoaderPtr& _loader, QWidget *parent = 0);
    ~ProjectChooser();

    void updateProjectList();
    void updateProjectListGui();

signals:
    void projectChoosen();

private slots:
    void projectClicked();

private:
    void clearGui();

private:
    Ui::ProjectChooser *m_ui;
    ProjectsLoaderPtr m_loader;

    QFileInfoList m_foundProjects;
    QMap<QPushButton*, QFileInfo> m_projectsBind;
};

#endif // PROJECTCHOOSER_H
