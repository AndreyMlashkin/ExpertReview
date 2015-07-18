#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treepropertywidget.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent),
     m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    m_ui->propertyWidget->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
    m_ui->propertyWidget->update();
    m_ui->propertyWidget->setResizeMode(QtTreePropertyBrowser::Interactive);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::resizeEvent(QResizeEvent *_ev)
{

}
