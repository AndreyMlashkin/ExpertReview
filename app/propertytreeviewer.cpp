#include <QFileInfo>

#include "propertytreeviewer.h"
#include "ui_propertytreeviewer.h"
#include "treepropertywidget.h"
#include "serialization/nodesinfo/treeleftsideinfofactory.h"
#include "serialization/nodesinfo/treerightsidevalues.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"

PropertyTreeViewer::PropertyTreeViewer(const ProjectsLoaderPtr &_loader, const QString &_leftSideTreeId, int _mode, QWidget *parent)
   : QWidget(parent),
     m_ui(new Ui::PropertyTreeViewer),
     m_loader(_loader),
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

    setDefaultTabName(m_leftInfo->defaultRightSideTreeName());
}

PropertyTreeViewer::~PropertyTreeViewer()
{
    // TODO
//    if(m_mode & SaveRegularOnExit)
//        writeRightSideVals();

    if(!isNormalised())
        saveValuesFromUi();

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
    {
        QString guiName = m_leftInfo->openRightSide(i)->guiName();
        m_ui->tabWidget->setTabText(i, guiName);
    }
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
        TreeRightSideValues* oldVals = m_loader->createRightSide(m_leftSideTreeId,
                                                                 "tmp", true);

        if(isServiceTab(m_currentTab))
            oldVals = m_treePropertyWidget->updateRightSideFromUi(oldVals);
        else
            oldVals = m_values[m_currentTab];
        Q_ASSERT(oldVals);

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
    m_leftInfo = m_loader->getLeftSideInfo(m_leftSideTreeId);
    m_treePropertyWidget = new TreePropertyWidget(m_leftInfo);

    m_average = m_ui->average;
    m_add = m_ui->add;

    QWidget::setWindowTitle(m_leftInfo->name());
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
    int tabsCount = m_ui->tabWidget->count();
    int insertPos = tabsCount - m_serviceTabsCount;

    QString tabName = generateTabName(insertPos);
    if(m_values.size() <= insertPos)
    {
        m_values.resize(insertPos + 1); // 1 - index to size;

        QString leftId = m_leftInfo->treeName();
        TreeRightSideValues *rightSide = m_loader->createRightSide(leftId);
        m_values[insertPos] = rightSide;
        QString guiName = rightSide->guiName();
        if(guiName.isEmpty())
            rightSide->setGuiName(tabName);
        else
            tabName = guiName;
    }
    m_ui->tabWidget->insertTab(insertPos, newWidget, tabName);
    m_ui->tabWidget->setCurrentWidget(newWidget);
}

bool PropertyTreeViewer::isNormalised() const
{
    return m_ui->normalise->isChecked();
}

TreeRightSideValues* PropertyTreeViewer::normalise(TreeRightSideValues *_values)
{
    QMap<QString, double> savedValues = _values->values();

    double summ = 0;
    foreach(double d, savedValues)
        summ += d;

    QMapIterator<QString, double> iter(savedValues);
    while (iter.hasNext())
    {
        iter.next();
        double val = iter.value();
        savedValues[iter.key()] = val / summ;
    }

    TreeRightSideValues* newVals = m_leftInfo->createRightSide();
    newVals->setValues(savedValues);
    return newVals;
}

TreeRightSideValues *PropertyTreeViewer::arithmeticalMean()
{
    QMap<QString, double> arMean;

    foreach (TreeRightSideValues* vals, m_values)
    {
        if(!vals)
            continue;

        QMapIterator<QString, double> iter(vals->values());
        while (iter.hasNext())
        {
            iter.next();
            QString key = iter.key();
            arMean[key] += iter.value();
        }
    }

    QMapIterator<QString, double> iter(arMean);
    while (iter.hasNext())
    {
        iter.next();
        QString key = iter.key();
        arMean[key] = iter.value() / m_values.size();
    }

    TreeRightSideValues* values = m_leftInfo->createRightSide();
    values->setValues(arMean);
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
        TreeRightSideValues* currentRValues = m_values[m_currentTab];
        m_treePropertyWidget->updateRightSideFromUi(currentRValues);
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
/*
void PropertyTreeViewer::writeRightSideVals()
{
    if(!isNormalised())
        saveValuesFromUi();
    //!!!
    int count = 0;
    foreach(TreeRightSideValues* rSide, m_values)
    {
        QString path = m_leftSideTreeId + QString::number(count++);
        rSide->writeValues(path);
    }        

    TreeRightSideValues* arMean = arithmeticalMean();
    arMean = normalise(arMean);
    arMean->writeValues(m_leftSideTreeId + "_average");
}*/

void PropertyTreeViewer::readRightSideVals()
{
    int count = m_leftInfo->savedRightSidesCount();
    for(int i = 0; i < count; ++i)
    {
        TreeRightSideValues* rightSide = m_leftInfo->openRightSide(i);
        if(rightSide->isTemp())
            continue;

        m_values << rightSide;
        addTab();
    }
}

