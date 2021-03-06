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

//!
//! \brief The ProjectChooser class класс графического интерфейса, позволяющий пользователю выбрать проект.
//!
class EXPERTREVIEW_EXPORT ProjectChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectChooser(const ProjectsLoaderPtr& _loader,
                            bool _inputExpertName = false,
                            const QString& _projectFileName = "project",
                            QWidget *parent = 0);
    ~ProjectChooser();

    //! \brief updateProjectList обновить список доступных проектов.
    void updateProjectList();
    void updateProjectListGui();
    void hideAddButton();
    void inputExpertName();

    QString getExpertName() const;

signals:
    void projectChoosen();
    void helpClicked();

protected slots:
    void projectClicked();
    void helpCalled();
    void addNewProject();

protected:
    void clearGui();
    void enableAllProjects(bool enable = true);
    bool eventFilter(QObject *obj, QEvent *event);
    QFileInfoList getProjectPatterns();
    void setupNewProject(const QString &patternName);

    static QDir projectPatternsDir();
    static bool copyDir(const QDir &src, const QDir &dest);

protected:
    Ui::ProjectChooser *m_ui;
    ProjectsLoaderPtr m_loader;

    bool m_newClicked;
    const QString m_projectFileName;
    QFileInfoList m_foundProjects;
    QMap<QPushButton*, QFileInfo> m_projectsBind;

    bool m_initialExpertNameTextSet;
};

#endif // PROJECTCHOOSER_H
