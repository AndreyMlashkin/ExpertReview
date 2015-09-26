#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class PropertyTreeViewer;
}

class PropertyTreeViewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit PropertyTreeViewer(QWidget *parent = 0);
    ~PropertyTreeViewer();

private slots:
    void tabChanged(int _newNum);

private:
    Ui::PropertyTreeViewer *m_ui;
};

#endif // MAINWINDOW_H
