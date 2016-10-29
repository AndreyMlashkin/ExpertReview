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

class EXPERTREVIEW_EXPORT ProjectChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectChooser(const ProjectsLoaderPtr& _loader, bool _inputExpertName = false, QWidget *parent = 0);
    ~ProjectChooser();

    void updateProjectList();
    void updateProjectListGui();
    void hideAddButton();
    void inputExpertName();

    QString getExpertName() const;

signals:
    void projectChoosen();

protected slots:
    void projectClicked();

protected:
    void clearGui();
    void enableAllProjects(bool enable = true);
    bool eventFilter(QObject *obj, QEvent *event);

protected:
    Ui::ProjectChooser *m_ui;
    ProjectsLoaderPtr m_loader;

    QFileInfoList m_foundProjects;
    QMap<QPushButton*, QFileInfo> m_projectsBind;

    bool m_initialExpertNameTextSet;
};

#endif // PROJECTCHOOSER_H
