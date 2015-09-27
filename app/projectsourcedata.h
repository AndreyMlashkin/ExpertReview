#ifndef PROJECTSOURCEDATA_H
#define PROJECTSOURCEDATA_H

#include <QWidget>

namespace Ui {
class ProjectSourceData;
}
class TreePropertyWidget;

class ProjectSourceData : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectSourceData(QWidget *parent = 0);
    ~ProjectSourceData();

private slots:
    void callCalculation();

private:
    Ui::ProjectSourceData *m_ui;
    TreePropertyWidget* m_tree;
};

#endif // PROJECTSOURCEDATA_H
