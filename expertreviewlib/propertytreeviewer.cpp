#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

#include "propertytreeviewer.h"
#include "projectcalculation.h"
#include "ui_propertytreeviewer.h"
#include "treepropertywidget.h"
#include "serialization/nodesinfo/treeleftsideinfofactory.h"
#include "serialization/nodesinfo/treerightsidevalues.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"

PropertyTreeViewer::PropertyTreeViewer(const ProjectsLoaderPtr &_loader, const QString &_leftSideTreeId,
                                       int _mode, QWidget *parent)
   : QWidget(parent),
     m_ui(new Ui::PropertyTreeViewer),
     m_loader(_loader),
     m_mode(_mode),
     m_average(nullptr),
     m_add(nullptr),
     m_import(nullptr),

     m_serviceTabsCount(0),

     m_leftSideTreeId(_leftSideTreeId),
     m_treePropertyWidget(nullptr),
     m_currentTab(0),
     m_factory(nullptr),
     m_leftInfo(nullptr)
{
    Q_ASSERT(!_loader.isNull());
    setAttribute(Qt::WA_DeleteOnClose);
    m_ui->setupUi(this);
    init();
    showMaximized();
    setMode(_mode);

    readRightSideVals();

    if(m_ui->tabWidget->count() == m_serviceTabsCount)
        addTab();
//  ----------------------
    Q_ASSERT(m_values.size() > 0);
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

    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)),   SLOT(tabChanged(int)));
    connect(m_ui->normalise, SIGNAL(clicked(bool)),         SLOT(normalise(bool)));
    connect(m_ui->tabWidget, SIGNAL(tabCloseRequested(int)),SLOT(removeExpertRequest(int)));

    setDefaultTabName(m_leftInfo->defaultRightSideTreeName());
}

PropertyTreeViewer::~PropertyTreeViewer()
{
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
    bool saveValuesFromUi = !isServiceTab(m_currentTab) && !isNormalised();
    tabChanged(_newNum, saveValuesFromUi);
}

void PropertyTreeViewer::tabChanged(int _newNum, bool _saveValuesFromUi)
{
    qDebug() << Q_FUNC_INFO << " new num = " << _newNum;
    QWidget* w = m_ui->tabWidget->widget(_newNum);
    if(!w)
        return;

    if(_saveValuesFromUi)
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
    else if(w == m_ui->import)
    {
        import();
        return;
    }
    else
    {
        qDebug() << m_values[_newNum]->guiName() << "\t"
                 << m_values[_newNum]->values();
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

        setPrecision(6);
    }
    else
    {
        if(m_ui->tabWidget->widget(m_currentTab) == m_ui->average)
            displayValuesForArithmeticalMean();
        else
            m_treePropertyWidget->setValues(m_values[m_currentTab]);

        setPrecision(2);
    }
}

void PropertyTreeViewer::removeExpertRequest(int _tabIndex)
{
    QString tabText = m_ui->tabWidget->tabText(_tabIndex);

    QMessageBox confirmation(this);
    confirmation.setWindowTitle("Удаление");
    confirmation.setText(QString("Вы действительно хотите удалить данные эксперта %1?").arg(tabText));


    int mode = QMessageBox::Yes | QMessageBox::No;
    confirmation.setStandardButtons(QMessageBox::StandardButton(mode));
    int buttonCode = confirmation.exec();

    qDebug() << Q_FUNC_INFO << " " << buttonCode;

    if(buttonCode == QMessageBox::Yes)
        removeTab(_tabIndex);
}

void PropertyTreeViewer::init()
{
    m_leftInfo = m_loader->getLeftSideInfo(m_leftSideTreeId);
    m_treePropertyWidget = new TreePropertyWidget(m_leftInfo);

    m_average = m_ui->average;
    m_add     = m_ui->add;
    m_import  = m_ui->import;

    m_ui->tabWidget->setTabToolTip(m_ui->tabWidget->indexOf(m_add), "Добавить");
    m_ui->tabWidget->setTabToolTip(m_ui->tabWidget->indexOf(m_import), "Импорт");

    QWidget::setWindowTitle(m_leftInfo->name());
    hideCloseButtonFromServiceTabs();
}

void PropertyTreeViewer::hideCloseButtonFromServiceTabs()
{
    QWidgetList serviceTabs { m_add, m_import, m_average };
    for(QWidget* serviceTab : serviceTabs)
    {
        int index_of_average = m_ui->tabWidget->indexOf(serviceTab);
        QWidget* closeButton = m_ui->tabWidget->tabBar()->tabButton(index_of_average, QTabBar::RightSide);
        closeButton->hide();
    }
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

    if(_mode & Import)
        ++m_serviceTabsCount;
    else
    {
        int importWgtIndex = m_ui->tabWidget->indexOf(m_import);
        m_ui->tabWidget->removeTab(importWgtIndex);
    }

    m_ui->tabWidget->setTabsClosable(_mode & TabsClosable);
}

void PropertyTreeViewer::addTab(const QString &_guiName)
{
	Q_ASSERT(m_leftInfo);

    QWidget* newWidget = new QWidget();
    int tabsCount = m_ui->tabWidget->count();
    int insertPos = tabsCount - m_serviceTabsCount;

    QString tabName;
    bool tabNameGenerated = _guiName.isEmpty();
    if(tabNameGenerated)
        tabName = generateTabName(insertPos);
    else
        tabName = _guiName;

    if(m_values.size() <= insertPos)
    {
        m_values.resize(insertPos + 1); // 1 - index to size;

        QString leftId = m_leftInfo->treeName();
        TreeRightSideValues *rightSide = m_loader->createRightSide(leftId, false, !tabNameGenerated);
        m_values[insertPos] = rightSide;
        QString guiName = rightSide->guiName();
        if(guiName.isEmpty())
            rightSide->setGuiName(tabName);
        else
            tabName = guiName;
    }
    m_ui->tabWidget->insertTab(insertPos, newWidget, tabName);
    m_ui->tabWidget->setCurrentWidget(newWidget);
    m_ui->tabWidget->setTabToolTip(insertPos, "");
}

void PropertyTreeViewer::removeTab(int _tabIndex)
{
    m_ui->tabWidget->blockSignals(true);

    QString tabText = m_ui->tabWidget->tabText(_tabIndex);
    TreeRightSideValues* rSide = m_leftInfo->getRightSides().at(_tabIndex);
    Q_ASSERT(rSide->guiName() == tabText);

    m_values.remove(_tabIndex);
    m_ui->tabWidget->removeTab(_tabIndex);
    m_loader->removeRightSide(rSide->id());

    if(_tabIndex == m_currentTab)
    {
        m_currentTab = m_currentTab?
                    m_currentTab-1 : m_currentTab;
        tabChanged(m_currentTab, false);
    }

    m_ui->tabWidget->blockSignals(false);
}

bool PropertyTreeViewer::isNormalised() const
{
    return m_ui->normalise->isChecked();
}

TreeRightSideValues* PropertyTreeViewer::normalise(TreeRightSideValues *_values)
{
    return ProjectCalculator::normalise(m_loader, _values);
}

TreeRightSideValues *PropertyTreeViewer::arithmeticalMean()
{
    return ProjectCalculator::getAverageRightSide(m_loader, m_leftInfo->treeName());
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
    if(!isServiceTab(m_currentTab) &&
            m_currentTab >= 0 && m_currentTab < m_values.size())
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
        l->setMargin(0);
        l->setSpacing(0);
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
    for(const QString& rSideName : m_loader->avaliableRightSides(m_leftSideTreeId))
    {
        addOneRSide(rSideName);
    }
}

void PropertyTreeViewer::addOneRSide(const QString &_rSideId)
{
    TreeRightSideValues* rSide = m_loader->getOrCreateRightSide(m_leftSideTreeId, _rSideId);
    m_values << rSide;
    addTab(rSide->guiName());
}

void PropertyTreeViewer::import()
{
    QString filePath = QFileDialog::getOpenFileName();
    QFileInfo fileInfo(filePath);
    if(!fileInfo.exists())
        return;

    QString fileName = fileInfo.fileName();
    QString newLocation(m_loader->projectDir() + "/" + fileName);
    if(QFileInfo(newLocation).exists())
    {
        // TODO AM add check. Some dialog or smth like that.
    }

    QFile::copy(filePath, newLocation);
    auto rSide = m_loader->getOrCreateRightSide(m_leftSideTreeId, fileName);
    addOneRSide(rSide->id());
}
