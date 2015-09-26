#include <QString>
#include <QLocale>

#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <QtTreePropertyBrowser>

#include "treepropertywidget.h"

#include "treemetainfo.h"
#include "properynode.h"

TreePropertyWidget::TreePropertyWidget(QWidget *_parent)
    : QtTreePropertyBrowser(_parent),
      m_variantManager(new QtVariantPropertyManager())
{
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    setFactoryForManager(m_variantManager, variantFactory);
    setPropertiesWithoutValueMarked(true);
    setRootIsDecorated(false);

    fillMetadata();
}

QString TreePropertyWidget::averageJudgeName()
{
    return "Average";
}

void TreePropertyWidget::fillMetadata()
{
    TreeMetaInfo metaInfo;
    const QList<ProperyNode *> nodes = metaInfo.nodes();

    foreach(ProperyNode* node, nodes)
        addProperty(toProperty(node));
}

void TreePropertyWidget::setCurrentJudge(const QString &_name)
{
    storeJudge(m_currentJudgeName);
    displayJudge(_name);
}

QString TreePropertyWidget::currentJudge()
{
    return m_currentJudgeName;
}

void TreePropertyWidget::setAverageCalculation()
{
    storeJudge(m_currentJudgeName);

    Judge average;

    QMapIterator<QString, Judge> j(m_associatedValues);
    while (j.hasNext())
    {
        j.next();
        if(j.key() == averageJudgeName())
            continue;

        QMapIterator<QtProperty*, QVariant> p(j.value());
        while (p.hasNext())
        {
            p.next();
            if(!average.contains(p.key()))
                average[p.key()] = 0.0;

            average[p.key()] = toDouble(p.value()) + toDouble(average[p.key()]);
        }
    }

    int count = m_associatedValues.count();
    QMapIterator<QtProperty*, QVariant> p(average);
    while (p.hasNext())
    {
        p.next();
        double val = toDouble(p.value());
        val /= count;
        average[p.key()] = QVariant(val);
    }
    displayJudge(average);
    m_currentJudgeName = averageJudgeName();
}

/*!
 * \brief TreePropertyWidget::storeJudge
 * Saves judge from ui with the name _name
 * \param _name
 */
void TreePropertyWidget::storeJudge(const QString &_name)
{
    if(_name.isEmpty() || _name == averageJudgeName())
        return;

    if(!m_associatedValues.contains(_name))
        m_associatedValues[_name] = Judge();

    Judge storingJudge = m_associatedValues.value(_name);

    foreach(QtProperty* prop, m_variantManager->properties())
        storingJudge[prop] = prop->valueText();

    m_associatedValues[_name] = storingJudge;
}

void TreePropertyWidget::displayJudge(const QString &_name)
{
    if(m_currentJudgeName == _name)
        return;

    if(!m_associatedValues.contains(_name))
        m_associatedValues[_name] = Judge();

    Judge displaying = m_associatedValues.value(_name);
    displayJudge(displaying);
    m_currentJudgeName = _name;
}

void TreePropertyWidget::displayJudge(const Judge &_judge)
{
    foreach(QtProperty* prop, m_variantManager->properties())
    {
        QVariant newValue = _judge.value(prop, QVariant(0.0d));
        double newDoubleVal = toDouble(newValue);
        m_variantManager->setValue(prop, newDoubleVal);
    }
}

QtProperty* TreePropertyWidget::toProperty(ProperyNode *_node)
{
    QtProperty *property = m_variantManager->addProperty(nodeType(_node), _node->description());

    foreach(ProperyNode* node, _node->children())
    {
        QtProperty* subProperty = toProperty(node);
        property->addSubProperty(subProperty);
    }

    return property;
}

int TreePropertyWidget::nodeType(const ProperyNode *_node) const
{
    bool hasChildren = !_node->children().isEmpty();
    if(hasChildren)
        return QtVariantPropertyManager::groupTypeId();
    else
        return QVariant::Double;
}

double TreePropertyWidget::toDouble(const QString &_str)
{
    if(_str.isEmpty())
        return -1;

    bool isOk;
    double ans = QLocale::system().toDouble(_str, &isOk);
    if(!isOk)
        ans = _str.toDouble(&isOk);
    Q_ASSERT(isOk);

    return ans;
}

double TreePropertyWidget::toDouble(const QVariant &_var)
{
    return toDouble(_var.toString());
}
