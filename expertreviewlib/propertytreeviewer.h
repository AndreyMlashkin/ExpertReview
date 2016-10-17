#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "serialization/projectsloader.h"

namespace Ui
{
    class PropertyTreeViewer;
}
class TreePropertyWidget;
class TreeLeftSideInfoFactory;
class TreeRightSideValues;
class TreeLeftSideInfo;

class EXPERTREVIEW_EXPORT PropertyTreeViewer : public QWidget
{
    Q_OBJECT
public:
    enum Mode { Minimal = 0,
                AddTabs = 0x1,
                AverageTab = 0x2,
                NormalizeButton = 0x4,

                All = 0xFFFFFFF
              };

    explicit PropertyTreeViewer(const ProjectsLoaderPtr& _loader,
                                const QString& _leftSideTreeId,
                                int _mode = All, QWidget *parent = 0);
    ~PropertyTreeViewer();

    void setDefaultTabName(const QString& _name);
    QString defaultTabName() const;

    void setPrecision(int _newPrecision);

private slots:
    void tabChanged(int _newNum);
    void normalise(bool _toggle);

private:
    void init();
    void setMode(int _mode);

    void addTab();
    bool isNormalised() const;
    TreeRightSideValues *normalise(TreeRightSideValues* _values);

    TreeRightSideValues* arithmeticalMean();
    void displayValuesForArithmeticalMean();

    QString generateTabName(int _num) const;
    bool isServiceTab(int _num) const;
    void saveValuesFromUi();

    void setActiveTab(int _tabNum);
    void setActiveTab(QWidget* _tab);

//    void writeRightSideVals();
    void readRightSideVals();

private:
    Ui::PropertyTreeViewer *m_ui;
    ProjectsLoaderPtr m_loader;

    int m_mode;
    QWidget* m_average;
    QWidget* m_add;
    int m_serviceTabsCount;

    QString m_defaultTabName;
    QString m_leftSideTreeId;

    TreePropertyWidget* m_treePropertyWidget;
    int m_currentTab;

    TreeLeftSideInfoFactory* m_factory;
    TreeLeftSideInfo* m_leftInfo;
    QVector<TreeRightSideValues*> m_values;
};

#endif // MAINWINDOW_H
