#ifndef MODECHOOSER_H
#define MODECHOOSER_H

#include <QWidget>

namespace Ui {
class ModeChooser;
}

class PropertyTreeViewer;
class ProjectSourceData;

class ModeChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ModeChooser(QWidget *parent = 0);
    ~ModeChooser();

private slots:
    void callMetodicJudges();
    void callCalculation();

private:
    Ui::ModeChooser *m_ui;
    PropertyTreeViewer* m_viewer;
    ProjectSourceData* m_sourceData;
};

#endif // MODECHOOSER_H
