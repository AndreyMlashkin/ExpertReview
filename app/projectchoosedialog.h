#ifndef PROJECTCHOOSEDIALOG_H
#define PROJECTCHOOSEDIALOG_H

#include <QWidget>
#include <QMap>
#include <QFileInfo>
#include <QFileInfo>

#include "serialization/projectsloader.h"

namespace Ui {
class ProjectChooseDialog;
}

class QPushButton;
class ProjectsLoader;

class ProjectChooseDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectChooseDialog(const ProjectsLoaderPtr& _loader, QWidget *parent = 0);
    ~ProjectChooseDialog();

    void updateProjectList();
    void updateProjectListGui();

signals:
    void projectChoosen(QFileInfo project);

private slots:
    void projectClicked();

private:
    void clearGui();

private:
    Ui::ProjectChooseDialog *m_ui;

    QFileInfoList m_foundProjects;
    QMap<QPushButton*, QFileInfo> m_projectsBind;
};

#endif // PROJECTCHOOSEDIALOG_H
