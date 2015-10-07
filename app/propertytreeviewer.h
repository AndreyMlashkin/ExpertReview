#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui
{
    class PropertyTreeViewer;
}
class TreePropertyWidget;
class TreeRightSideValues;
class TreeLeftSideInfo;

// нужно сделать инъекцию зависимости TreePropertyWidget - получать в конструктре
class PropertyTreeViewer : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyTreeViewer(QWidget *parent = 0);
    ~PropertyTreeViewer();

private slots:
    void tabChanged(int _newNum);
    void normalise(bool _toggle);

private:
    void addTab();
    bool normalise() const;

private:
    Ui::PropertyTreeViewer *m_ui;
    TreePropertyWidget* m_treePropertyWidget;

    TreeLeftSideInfo* m_info;
    QList<TreeRightSideValues*> m_values;
};

#endif // MAINWINDOW_H
