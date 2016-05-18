#ifndef MODECHOOSER_H
#define MODECHOOSER_H

#include "memory"
#include <QWidget>

namespace Ui {
class ModeChooser;
}

class ProjectsLoader;

class ModeChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ModeChooser(const std::shared_ptr<ProjectsLoader>& _loader, QWidget *parent = 0);
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
    struct ModeChooserPrivate;
    ModeChooserPrivate* p;

    std::shared_ptr<ProjectsLoader> m_loader;
};

#endif // MODECHOOSER_H
