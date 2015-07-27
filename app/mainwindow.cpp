#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treepropertywidget.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent),
     m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    showMaximized();

    m_ui->propertyWidget->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
    m_ui->propertyWidget->update();
    m_ui->propertyWidget->setResizeMode(QtTreePropertyBrowser::Interactive);


    QFont font = m_ui->propertyWidget->font();
    font.setPointSize(12);
    m_ui->propertyWidget->setFont(font);
    m_ui->propertyWidget->setSplitterPosition(double(width()) / 0.5);

    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));

   m_ui->propertyWidget->setCurrentJudge(m_ui->tabWidget->tabText(0));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::tabChanged(int _newNum)
{
    QWidget* w = m_ui->tabWidget->widget(_newNum);
    if(!w)
        return;

    if(w == m_ui->average)
    {
        m_ui->propertyWidget->setAverageCalculation();
        w->layout()->addWidget(m_ui->propertyWidget);
        return;
    }
    else if(w == m_ui->add)
    {
        QWidget* newWidget = new QWidget();
        int tabsCount = m_ui->tabWidget->indexOf(m_ui->add);
        static int judgeCount = 1;
        m_ui->tabWidget->insertTab(tabsCount, newWidget, QString("Judge") + QString::number(++judgeCount));
        m_ui->tabWidget->setCurrentWidget(newWidget);
        return;
    }    
    if(!w->layout())
    {
        QHBoxLayout* l = new QHBoxLayout();
        w->setLayout(l);
    }
    w->layout()->addWidget(m_ui->propertyWidget);

    m_ui->propertyWidget->setCurrentJudge(m_ui->tabWidget->tabText(_newNum));
}
