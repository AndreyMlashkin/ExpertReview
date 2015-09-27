#include "propertytreeviewer.h"
#include "ui_propertytreeviewer.h"
#include "treepropertywidget.h"

PropertyTreeViewer::PropertyTreeViewer(QWidget *parent)
   : QMainWindow(parent),
     m_ui(new Ui::PropertyTreeViewer),
     m_treePropertyWidget(new TreePropertyWidget())
{
    m_ui->setupUi(this);
    showMaximized();

    m_ui->defaultTab->layout()->addWidget(m_treePropertyWidget);

    m_treePropertyWidget->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
    m_treePropertyWidget->update();
    m_treePropertyWidget->setResizeMode(QtTreePropertyBrowser::Interactive);

    QFont font = m_treePropertyWidget->font();
    font.setPointSize(12);
    m_treePropertyWidget->setFont(font);
    m_treePropertyWidget->setSplitterPosition(double(width()) / 0.5);

    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));

   m_treePropertyWidget->setCurrentJudge(m_ui->tabWidget->tabText(0));
}

PropertyTreeViewer::~PropertyTreeViewer()
{
    delete m_ui;
}

void PropertyTreeViewer::tabChanged(int _newNum)
{
    QWidget* w = m_ui->tabWidget->widget(_newNum);
    if(!w)
        return;

    if(w == m_ui->average)
    {
        m_treePropertyWidget->setAverageCalculation();
        w->layout()->addWidget(m_treePropertyWidget);
        return;
    }
    else if(w == m_ui->add)
    {
        QWidget* newWidget = new QWidget();
        int tabsCount = m_ui->tabWidget->indexOf(m_ui->add);
        static int judgeCount = 1;
        m_ui->tabWidget->insertTab(tabsCount, newWidget, QString("Эксперт") + QString::number(++judgeCount));
        m_ui->tabWidget->setCurrentWidget(newWidget);
        return;
    }    
    if(!w->layout())
    {
        QHBoxLayout* l = new QHBoxLayout();
        w->setLayout(l);
    }
    w->layout()->addWidget(m_treePropertyWidget);

    m_treePropertyWidget->setCurrentJudge(m_ui->tabWidget->tabText(_newNum));
}
