#include <QMap>
#include <QLocale>

#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <QtTreePropertyBrowser>

#include "treepropertywidget.h"

#include "projectapi.h"
#include "nodesinfo/treeleftsideinfofile.h"
#include "nodesinfo/treerightsidevaluesfile.h"
#include "nodesinfo/treeleftsideinfofactory.h"
#include "properynode.h"

TreePropertyWidget::TreePropertyWidget(TreeLeftSideInfo* _info, QWidget *_parent)
    : QtTreePropertyBrowser(_parent),
      m_leftSide(_info),
      m_normalised(false),
      m_precision(2),
      m_variantManager(new QtVariantPropertyManager())
{
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    setFactoryForManager(m_variantManager, variantFactory);
    setPropertiesWithoutValueMarked(true);
    setRootIsDecorated(false);

    fillLeftSide();
}

inline QtProperty* findPropery(QtVariantPropertyManager* _mananger, const QString& _key)
{
    foreach(QtProperty* prop, _mananger->properties())
    {
        if(prop->propertyName() == _key)
            return prop;
    }
    return NULL;
}

void TreePropertyWidget::setValues(TreeRightSideValues *_values)
{
//    Q_ASSERT(_values->values().size() == m_info->planeNodes().size());

    if(!_values || _values->values().isEmpty())
    {
        clear();
        return;
    }

    QStringList orderedKeys = m_leftSide->planeKeys();
    QMap<QString, double> values = _values->values();
    foreach(QString key, orderedKeys)
    {
        double newValue = values[key];
        QtProperty* properyToChange = m_planeProperties[key];
//        m_variantManager->setAttribute(prop, "decimals", QVariant(m_precision));
        m_variantManager->setValue(properyToChange, newValue);
    }
}

TreeRightSideValues *TreePropertyWidget::getValues() const
{
    QMap<QString, double> values;

    QMapIterator<QString, QtProperty*> iter = m_planeProperties;
    while(iter.hasNext())
    {
        iter.next();
        QtProperty* prop = iter.value();
        QVariant varValue = m_variantManager->value(prop);
        double value = toDouble(varValue);
        QString key = iter.key();
        values[key] = value;
    }

    TreeRightSideValues* rightSide = m_leftSide->createRightSide();
    rightSide->setValues(values);
    return rightSide;

//    QVariantList values;
//    QStringList orderedKeys = m_leftSide->planeDescriptions();
//    foreach (QString key, orderedKeys)
//    {
//        QtProperty* prop = findPropery(m_variantManager, key);
//        values << m_variantManager->value(prop);
//    }

//    TreeRightSideValues* ans = m_leftSide->createRightSide();
//    ans->setValues(values);
//    return ans;
}

void TreePropertyWidget::clear()
{
    foreach (QtProperty* prop, m_variantManager->properties())
        m_variantManager->setValue(prop, QVariant(0.0d));
}

void TreePropertyWidget::fillLeftSide()
{
    const QList<ProperyNode *> nodes = m_leftSide->nodes();
    foreach(ProperyNode* node, nodes)
        addProperty(toProperty(node));
}

QtProperty* TreePropertyWidget::toProperty(ProperyNode *_node)
{
    QtProperty *property = m_variantManager->addProperty(nodeType(_node), _node->description());
    m_planeProperties[_node->key()] = property;

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

//double TreePropertyWidget::toDouble(const QString &_str) const
//{
//    if(_str.isEmpty())
//        return 0;

//    bool isOk;
//    double ans = QLocale::system().toDouble(_str, &isOk);
//    if(!isOk)
//        ans = _str.toDouble(&isOk);
//    Q_ASSERT(isOk);

//    return ans;
//}

//double TreePropertyWidget::toDouble(const QVariant &_var) const
//{
//    return toDouble(_var.toString());
//}

void TreePropertyWidget::setEditable(bool _set)
{
    foreach(QtProperty* prop, m_variantManager->properties())
        prop->setEnabled(_set);
}

void TreePropertyWidget::setPrecision(int _newPrecision)
{
    m_precision = _newPrecision;
    foreach(QtProperty* prop, m_variantManager->properties())
    {
        m_variantManager->setAttribute(prop, "decimals", QVariant(_newPrecision));
        m_variantManager->setValue(prop, toDouble(prop->valueText()));
    }

    update();
}
