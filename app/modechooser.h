#ifndef MODECHOOSER_H
#define MODECHOOSER_H

#include <QWidget>
#include "serialization/projectsloader.h"

namespace Ui {
class ModeChooser;
}

class ProjectsLoader;

class ModeChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ModeChooser(const ProjectsLoaderPtr &_loader, QWidget *parent = 0);
    ~ModeChooser();

private slots:
    void callMetodicJudges();
    void callSectionJudges();
    void callSourceData();
    void callSectionCalculation();
    void callCalculation();

private:
    void updateResult();
    void closeEvent(QCloseEvent * event) override;

private:
    Q_DISABLE_COPY(ModeChooser);

    Ui::ModeChooser *m_ui;
    ProjectsLoaderPtr m_loader;
    struct ModeChooserPrivate;
    ModeChooserPrivate* p;

};

#endif // MODECHOOSER_H
