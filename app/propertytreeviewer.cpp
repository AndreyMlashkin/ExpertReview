#include <QDebug>
#include <QFileInfo>

#include "propertytreeviewer.h"
#include "ui_propertytreeviewer.h"
#include "treepropertywidget.h"
#include "nodesinfo/treeinfofactory.h"
#include "nodesinfo/treerightsidevalues.h"
#include "nodesinfo/treeleftsideinfo.h"

const int SERVICE_TABS_COUNT = 2;

PropertyTreeViewer::PropertyTreeViewer(const QString &_treeId, QWidget *parent)
   : QWidget(parent),
     m_ui(new Ui::PropertyTreeViewer),
     m_treeId(_treeId),
     m_treePropertyWidget(NULL),
     m_currentTab(0),
     m_factory(NULL),
     m_leftInfo(NULL)
{
    setAttribute(Qt::WA_DeleteOnClose);

    init();

    m_ui->setupUi(this);
    showMaximized();

    readRightSideVals();

    if(m_ui->tabWidget->count() == SERVICE_TABS_COUNT)
        addTab();
//  ----------------------
    m_treePropertyWidget->setValues(m_values[0]);
    m_currentTab = 0;
    setActiveTab(0);
//  ----------------------

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
    writeRightSideVals();
    delete m_ui;
}

void PropertyTreeViewer::setDefaultTabName(const QString &_name)
{
    m_defaultTabName = _name;

    int tabsCount = m_ui->tabWidget->count();

    for(int i = 0; i < tabsCount - SERVICE_TABS_COUNT; ++i)
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

    if(!isServiceTab(m_currentTab) && !isNormalised())
        saveValuesFromUi();

    if(w == m_ui->average)
    {
        displayValuesForArithmeticalMean();
        w->layout()->addWidget(m_treePropertyWidget);
        m_currentTab = _newNum;
    }
    else if(w == m_ui->add)
    {
        addTab();
        return;
    }    
    else
    {
        m_treePropertyWidget->setValues(m_values[_newNum]);
        m_currentTab = _newNum;
    }

    setActiveTab(w);
    normalise(isNormalised());
}

void PropertyTreeViewer::normalise(bool _toggled)
{
    m_treePropertyWidget->setEditable(!_toggled);

    if(_toggled)
    {
        saveValuesFromUi();
        TreeRightSideValues* oldVals = NULL;

        if(isServiceTab(m_currentTab))
            oldVals = m_treePropertyWidget->getValues();
        else
            oldVals = m_values[m_currentTab];
        Q_ASSERT(oldVals);

        QVariantList savedValues = oldVals->values();

        double summ = 0;
        foreach(QVariant v, savedValues)
            summ += v.value<double>();

        for(int i = 0; i < savedValues.size(); ++i)
        {
            double val = savedValues.at(i).value<double>();
            savedValues[i] = val / summ;
        }

        TreeRightSideValues* newVals = m_factory->getRightSideValues();
        newVals->setValues(savedValues);
        m_treePropertyWidget->setValues(newVals);
    }
    else
    {
        if(m_ui->tabWidget->widget(m_currentTab) == m_ui->average)
            displayValuesForArithmeticalMean();
        else
            m_treePropertyWidget->setValues(m_values[m_currentTab]);
    }
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
    int insertPos = tabsCount - SERVICE_TABS_COUNT;

    m_ui->tabWidget->insertTab(insertPos, newWidget, generateTabName(insertPos));

    if(m_values.size() <= insertPos)
    {
        m_values.resize(insertPos + 1); // 1 - index to size;
        m_values[insertPos] = m_factory->getRightSideValues();
    }
    m_ui->tabWidget->setCurrentWidget(newWidget);
}

bool PropertyTreeViewer::isNormalised() const
{
    return m_ui->normalise->isChecked();
}

void PropertyTreeViewer::displayValuesForArithmeticalMean()
{
    QVector<double> arMean;
    arMean.resize(m_leftInfo->planeNodes().size());

    foreach (TreeRightSideValues* vals, m_values)
    {
        if(!vals)
            continue;

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

void PropertyTreeViewer::saveValuesFromUi()
{
    if(!isServiceTab(m_currentTab))
    {
        TreeRightSideValues* previousVals = m_treePropertyWidget->getValues();
        delete m_values[m_currentTab];
        m_values[m_currentTab] = previousVals;
    }
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

void PropertyTreeViewer::writeRightSideVals()
{
    if(!isNormalised())
        saveValuesFromUi();
    int count = 0;
    foreach(TreeRightSideValues* rSide, m_values)
    {
        QString path = m_treeId + QString::number(count++);
        rSide->writeValues(path);
    }
}

void PropertyTreeViewer::readRightSideVals()
{
    //!!!
    int count = 0;
    forever
    {
        QString path = m_treeId + QString::number(count++);
        QFileInfo info(path);
        if(info.exists())
        {
            m_values << m_factory->getRightSideValues(path);
            qDebug() << m_values.size();
            addTab();
            qDebug() << m_values.size();
        }
        else
            break;
    }
}

