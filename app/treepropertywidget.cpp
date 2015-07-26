#include <QDebug>
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

void TreePropertyWidget::fillMetadata()
{
    TreeMetaInfo metaInfo;
    const QList<ProperyNode *> nodes = metaInfo.nodes();

    foreach(ProperyNode* node, nodes)
        addProperty(toProperty(node));
}

void TreePropertyWidget::setCurrentJudge(const QString _name)
{
    if(!m_associatedValues.contains(_name))
        m_associatedValues[_name] = Judge();

    Judge oldJudge = m_associatedValues.value(m_currentJudgeName);
    Judge newJudge = m_associatedValues.value(_name);

    foreach(QtProperty* prop, m_variantManager->properties())
    {
        double valueToSave = toDouble(prop->valueText());
        oldJudge[prop] = valueToSave;

        QVariant newValue = newJudge.value(prop, QVariant(0.0d));
        double newDoubleVal = toDouble(newValue);
        m_variantManager->setValue(prop, newDoubleVal);
    }
    m_associatedValues[m_currentJudgeName] = oldJudge;
    m_currentJudgeName = _name;
}

QString TreePropertyWidget::currentJudge()
{
    return m_currentJudgeName;
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

    QLocale rus(QLocale::Russian);
    bool isOk;
    double ans = rus.toDouble(_str, &isOk);
    Q_ASSERT(isOk);
    return ans;
}

double TreePropertyWidget::toDouble(const QVariant &_var)
{
    return toDouble(_var.toString());
}
