#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "serialization/projectsloader.h"

namespace Ui
{
    class PropertyTreeViewer;
}
class TreePropertyWidget;
class TreeRightSideValues;
class TreeLeftSideInfo;
class FullTreeTableModel;
class QTableView;

//!
//! \brief The PropertyTreeViewer class класс графического интерфейса, позволяющий пользователю
//! работать с проектом, а также добавлять и импортировать значения проектов, нормализировать значения и
//! просматривать усреднённое значение по всем проектам.
//!
class EXPERTREVIEW_EXPORT PropertyTreeViewer : public QWidget
{
    Q_OBJECT
public:
    enum Mode { Minimal         = 0,
                AddTabs         = 0x1,
                ImportTab       = 0x2,
                AverageTab      = 0x4,
                NormalizeButton = 0x8,
                TabsClosable    = 0x10,
                FinalCastTab    = 0x20,

                All = 0xFFFFFFF
              };

    explicit PropertyTreeViewer(const ProjectsLoaderPtr& _loader,
                                const QString& _leftSideTreeId,
                                int _mode = All, QWidget *parent = 0);
    ~PropertyTreeViewer();

    void setDefaultTabName(const QString& _name);
    QString defaultTabName() const;

    void setPrecision(int _newPrecision);
    void setNormalizedTabPrecision(int _newPrecision);

    void setMaxValue (int _max);
    void setMinValue (int _min);

private slots:
    void tabChanged(int _newNum);
    void tabChanged(int _newNum, bool _saveValuesFromUi);

    void normalise(bool _toggle);
    void removeExpertRequest(int _tabIndex);

private:
    void init();
    void hideCloseButtonFromServiceTabs();
    void setMode(int _mode);

    void addTab(const QString& _guiName = "");
    void removeTab(int _tabIndex);

    bool isNormalised() const;
    TreeRightSideValues *normalise(TreeRightSideValues* _values);

    TreeRightSideValues* arithmeticalMean();
    void displayValuesForArithmeticalMean();

    void displayFinalCast();

    QString generateTabName(int _num) const;
    bool isServiceTab(int _num) const;
    void saveValuesFromUi();

    void setActiveTab(int _tabNum);
    void setActiveTab(QWidget* _tab);

//    void writeRightSideVals();
    void readRightSideVals();
    void addOneRSide(const QString& _rSideId);

    void import();

    void updateToolTipsIndexes();

private:
    Ui::PropertyTreeViewer *m_ui;
    ProjectsLoaderPtr m_loader;

    int m_mode;
    int m_precision;
    int m_normalisedTabPrecision;

    QWidget* m_average;
    QWidget* m_add;
    QWidget* m_import;
    QWidget* m_finalCast;
    int m_serviceTabsCount;

    FullTreeTableModel* m_fullModel;
    QTableView* m_fullView;

    QString m_defaultTabName;
    QString m_leftSideTreeId;

    TreePropertyWidget* m_treePropertyWidget;
    int m_currentTab;

    TreeLeftSideInfo* m_leftInfo;
    QVector<TreeRightSideValues*> m_values;
};

#endif // MAINWINDOW_H
