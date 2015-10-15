#include <QFileInfo>

#include "propertytreeviewer.h"
#include "ui_propertytreeviewer.h"
#include "treepropertywidget.h"
#include "nodesinfo/treeinfofactory.h"
#include "nodesinfo/treerightsidevalues.h"
#include "nodesinfo/treeleftsideinfo.h"

PropertyTreeViewer::PropertyTreeViewer(const QString &_leftSideTreeId, int _mode, QWidget *parent)
   : QWidget(parent),
     m_ui(new Ui::PropertyTreeViewer),
     m_mode(_mode),
     m_serviceTabsCount(0),

     m_leftSideTreeId(_leftSideTreeId),
     m_treePropertyWidget(NULL),
     m_currentTab(0),
     m_factory(NULL),
     m_leftInfo(NULL)
{
    setAttribute(Qt::WA_DeleteOnClose);
    m_ui->setupUi(this);
    init();
    showMaximized();
    setMode(_mode);

    readRightSideVals();

    if(m_ui->tabWidget->count() == m_serviceTabsCount)
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
    if(m_mode & SaveRegularOnExit)
        writeRightSideVals();
    delete m_ui;
}

//PropertyTreeViewer::Mode PropertyTreeViewer::mode() const
//{
//    int currMode = 0x0;
//    if(m_ui->add->isVisible())
//        currMode |= AddTabs;
//    if(m_ui->average->isVisible())
//        currMode |= AverageTab;
//    if(m_ui->normalise->isVisible())
//        currMode |= NormalizeButton;

//    return Mode(currMode);
//}

void PropertyTreeViewer::setDefaultTabName(const QString &_name)
{
    m_defaultTabName = _name;

    int tabsCount = m_ui->tabWidget->count();

    for(int i = 0; i < tabsCount - m_serviceTabsCount; ++i)
        m_ui->tabWidget->setTabText(i, generateTabName(i));
}

QString PropertyTreeViewer::defaultTabName() const
{
    return m_defaultTabName;
}

void PropertyTreeViewer::setPrecision(int _newPrecision)
{
    m_treePropertyWidget->setPrecision(_newPrecision);
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

//        QVariantList savedValues = oldVals->values();

//        double summ = 0;
//        foreach(QVariant v, savedValues)
//            summ += v.value<double>();

//        for(int i = 0; i < savedValues.size(); ++i)
//        {
//            double val = savedValues.at(i).value<double>();
//            savedValues[i] = val / summ;
//        }

//        TreeRightSideValues* newVals = m_factory->getRightSideValues();
//        newVals->setValues(savedValues);

        TreeRightSideValues* newVals = normalise(oldVals);
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
    m_leftInfo = m_factory->getLeftSideInfo(m_leftSideTreeId);
    m_treePropertyWidget = new TreePropertyWidget(m_leftInfo, m_factory);

    m_average = m_ui->average;
    m_add = m_ui->add;
}

void PropertyTreeViewer::setMode(int _mode)
{
    m_serviceTabsCount = 0;

    m_ui->normalise->setVisible(_mode & NormalizeButton);

    if(_mode & AddTabs)
        ++m_serviceTabsCount;
    else
    {
        int addWgtIndex = m_ui->tabWidget->indexOf(m_add);
        m_ui->tabWidget->removeTab(addWgtIndex);
    }

    if(_mode & AverageTab)
        ++m_serviceTabsCount;
    else
    {
        int averageWgtIndex = m_ui->tabWidget->indexOf(m_average);
        m_ui->tabWidget->removeTab(averageWgtIndex);
    }
}

void PropertyTreeViewer::addTab()
{
    QWidget* newWidget = new QWidget();
//    int tabsCount = m_ui->tabWidget->indexOf(m_ui->add);
    int tabsCount = m_ui->tabWidget->count();
    int insertPos = tabsCount - m_serviceTabsCount;

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

TreeRightSideValues* PropertyTreeViewer::normalise(TreeRightSideValues *_values)
{
    QVariantList savedValues = _values->values();

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
    return newVals;
}

TreeRightSideValues *PropertyTreeViewer::arithmeticalMean()
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
    return values;
}

void PropertyTreeViewer::displayValuesForArithmeticalMean()
{
    TreeRightSideValues* arMean = arithmeticalMean();
    m_treePropertyWidget->setValues(arMean);
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
        QString path = m_leftSideTreeId + QString::number(count++);
        rSide->writeValues(path);
    }        

    TreeRightSideValues* arMean = arithmeticalMean();
    arMean = normalise(arMean);
    arMean->writeValues(m_leftSideTreeId + "_average");
}

void PropertyTreeViewer::readRightSideVals()
{
    //!!!
    int count = 0;
    forever
    {
        QString path = m_leftSideTreeId + QString::number(count++);
        QFileInfo info(path);
        if(info.exists())
        {
            m_values << m_factory->getRightSideValues(path);
            addTab();
        }
        else
            break;
    }
}

