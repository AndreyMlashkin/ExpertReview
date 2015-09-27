#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui
{
    class PropertyTreeViewer;
}
class TreePropertyWidget;

class PropertyTreeViewer : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyTreeViewer(QWidget *parent = 0);
    ~PropertyTreeViewer();

signals:
    void closing();

private slots:
    void tabChanged(int _newNum);
    void normalise(bool _toggle);

private:
    bool normalise() const;

private:
    Ui::PropertyTreeViewer *m_ui;
    TreePropertyWidget* m_treePropertyWidget;
};

#endif // MAINWINDOW_H
