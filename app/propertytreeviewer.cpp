#include "propertytreeviewer.h"
#include "ui_propertytreeviewer.h"
#include "treepropertywidget.h"
#include "nodesinfo/treeinfofactory.h"

PropertyTreeViewer::PropertyTreeViewer(const QString &_treeId, QWidget *parent)
   : QWidget(parent),
     m_ui(new Ui::PropertyTreeViewer),
     m_treePropertyWidget(new TreePropertyWidget("factors.txt")),
     m_factory(new TreeInfoFactory()),
     m_info(m_factory->getLeftSideInfo(_treeId))
   // как загрузить и значения?
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
    connect(m_ui->normalise, SIGNAL(clicked(bool)),       SLOT(normalise(bool)));

    m_treePropertyWidget->setCurrentJudge(m_ui->tabWidget->tabText(0),  normalise());
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
        m_treePropertyWidget->setAverageCalculation(normalise());
        w->layout()->addWidget(m_treePropertyWidget);
        return;
    }
    else if(w == m_ui->add)
    {
        addTab();
        return;
    }    
    if(!w->layout())
    {
        QHBoxLayout* l = new QHBoxLayout();
        w->setLayout(l);
    }
    w->layout()->addWidget(m_treePropertyWidget);

    m_treePropertyWidget->setCurrentJudge(m_ui->tabWidget->tabText(_newNum), normalise());
}

void PropertyTreeViewer::normalise(bool _toggled)
{
    m_treePropertyWidget->normalise(_toggled);
}

void PropertyTreeViewer::addTab()
{
    QWidget* newWidget = new QWidget();
    int tabsCount = m_ui->tabWidget->indexOf(m_ui->add);
    static int judgeCount = 1;
    m_ui->tabWidget->insertTab(tabsCount, newWidget, QString("Эксперт") + QString::number(++judgeCount));
    m_ui->tabWidget->setCurrentWidget(newWidget);
}

bool PropertyTreeViewer::normalise() const
{
    return m_ui->normalise->isChecked();
}

