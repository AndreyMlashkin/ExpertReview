#include "propertytreeviewer.h"
#include "ui_propertytreeviewer.h"
#include "treepropertywidget.h"
#include "nodesinfo/treeinfofactory.h"
#include "nodesinfo/treerightsidevalues.h"
#include "nodesinfo/treeleftsideinfo.h"

PropertyTreeViewer::PropertyTreeViewer(const QString &_treeId, QWidget *parent)
   : QWidget(parent),
     m_ui(new Ui::PropertyTreeViewer),
     m_treeId(_treeId),
     m_treePropertyWidget(NULL),
     m_currentTab(0),
     m_factory(NULL),
     m_leftInfo(NULL)
{
    init();

    m_ui->setupUi(this);
    showMaximized();

    addTab();
    setActiveTab(0);

    m_treePropertyWidget->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
    m_treePropertyWidget->update();
    m_treePropertyWidget->setResizeMode(QtTreePropertyBrowser::Interactive);

    QFont font = m_treePropertyWidget->font();
    font.setPointSize(12);
    m_treePropertyWidget->setFont(font);
    m_treePropertyWidget->setSplitterPosition(double(width()) / 0.5);

    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));
    connect(m_ui->normalise, SIGNAL(clicked(bool)),       SLOT(normalise(bool)));
}

PropertyTreeViewer::~PropertyTreeViewer()
{
    delete m_ui;
}

void PropertyTreeViewer::setDefaultTabName(const QString &_name)
{
    m_defaultTabName = _name;

    int tabsCount = m_ui->tabWidget->count();

    for(int i = 0; i < tabsCount - 2; ++i)
        m_ui->tabWidget->setTabText(i, generateTabName(i));
}

QString PropertyTreeViewer::defaultTabName() const
{
    return m_defaultTabName;
}

void PropertyTreeViewer::tabChanged(int _newNum)
{
    QWidget* w = m_ui->tabWidget->widget(_newNum);
    if(!w)
        return;

    if(w == m_ui->average)
    {
        updateArithmeticalMean();
        w->layout()->addWidget(m_treePropertyWidget);
        m_currentTab = _newNum;
        return;
    }
    else if(w == m_ui->add)
    {
        addTab();
        return;
    }    
    else
    {
        if(!isServiceTab(m_currentTab))
        {
            TreeRightSideValues* previousVals = m_treePropertyWidget->getValues();
            delete m_values[m_currentTab];
            m_values[m_currentTab] = previousVals;
        }

        m_treePropertyWidget->setValues(m_values[_newNum]);
        m_currentTab = _newNum;
    }

    setActiveTab(w);
}

void PropertyTreeViewer::normalise(bool _toggled)
{
    m_treePropertyWidget->setEditable(!_toggled);
}

void PropertyTreeViewer::init()
{
    m_factory = new TreeInfoFactory();
    m_leftInfo = m_factory->getLeftSideInfo(m_treeId);
    m_treePropertyWidget = new TreePropertyWidget(m_leftInfo, m_factory);
}

void PropertyTreeViewer::addTab()
{
    QWidget* newWidget = new QWidget();
//    int tabsCount = m_ui->tabWidget->indexOf(m_ui->add);
    int tabsCount = m_ui->tabWidget->count();
    int insertPos = tabsCount - 2;

    m_ui->tabWidget->insertTab(insertPos, newWidget, generateTabName(insertPos));

    if(m_values.size() <= insertPos)
    {
        m_values.resize(insertPos + 1);
        m_values[insertPos] = m_factory->getRightSideValues();
    }
    m_ui->tabWidget->setCurrentWidget(newWidget);
}

bool PropertyTreeViewer::normalise() const
{
    return m_ui->normalise->isChecked();
}

void PropertyTreeViewer::updateArithmeticalMean()
{
    QVector<double> arMean;
    arMean.resize(m_leftInfo->planeNodes().size());

    foreach (TreeRightSideValues* vals, m_values)
    {
        for(int i = 0; i < vals->values().size(); ++i)
        {
            double digit = vals->values().at(i).value<double>();
            arMean[i] += digit;
        }
    }

    QVariantList ans;
    for(int i = 0; i < arMean.size(); ++i)
        ans << double(arMean.at(i) / m_values.size());

    TreeRightSideValues* values = m_factory->getRightSideValues();
    values->setValues(ans);
    m_treePropertyWidget->setValues(values);
}

QString PropertyTreeViewer::generateTabName(int _num) const
{
    return m_defaultTabName + " " + QString::number(_num + 1);
}

bool PropertyTreeViewer::isServiceTab(int _num) const
{
    QWidget* tab = m_ui->tabWidget->widget(_num);
    return (tab == m_ui->add || tab == m_ui->average);
}

void PropertyTreeViewer::setActiveTab(int _tabNum)
{
    QWidget* tab = m_ui->tabWidget->widget(_tabNum);
    setActiveTab(tab);
}

void PropertyTreeViewer::setActiveTab(QWidget *_tab)
{
    if(!_tab->layout())
    {
        QHBoxLayout* l = new QHBoxLayout();
        _tab->setLayout(l);
    }
    _tab->layout()->addWidget(m_treePropertyWidget);
    m_ui->tabWidget->setCurrentWidget(_tab);
}

