#include <QDebug>
#include <QMap>
#include <QLocale>

#include <include/qtpropertybrowser/QtVariantPropertyManager>
#include <include/qtpropertybrowser/QtVariantProperty>
#include <include/qtpropertybrowser/QtTreePropertyBrowser>

#include "treepropertywidget.h"

#include "projectapi.h"
#include "serialization/nodesinfo/treeleftsideinfo.h"
#include "serialization/nodesinfo/treerightsidevalues.h"
#include "serialization/nodesinfo/treeleftsideinfofactory.h"
#include "serialization/nodesinfo/propertynode.h"

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

TreePropertyWidget::~TreePropertyWidget()
{
    delete m_variantManager;
}

inline QtProperty* findPropery(QtVariantPropertyManager* _mananger, const QString& _key)
{
    for(QtProperty* prop : _mananger->properties())
    {
        if(prop->propertyName() == _key)
            return prop;
    }
    return nullptr;
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
    QList<int> minValues = m_leftSide->planeMinValues();
    QList<int> maxValues = m_leftSide->planeMaxValues();
    QMap<QString, double> values = _values->values();

    Q_ASSERT(maxValues.size() == minValues.size()&&
             maxValues.size() == orderedKeys.size());

    for(int i = 0; i < orderedKeys.size(); ++i)
    {
        const QString& key = orderedKeys.at(i);
        QtProperty* prop = m_planeProperties[key];
        int newMaxValue = maxValues[i];
        newMaxValue = newMaxValue? newMaxValue : INT32_MAX;
        m_variantManager->setAttribute(prop , "maximum", QVariant(newMaxValue));
        m_variantManager->setAttribute(prop , "minimum", QVariant(minValues[i]));
//        m_variantManager->setAttribute(prop, "decimals", QVariant(m_precision));
        double newValue = values[key];
        m_variantManager->setValue(prop , newValue);
    }
    setPrecision(m_precision);
}

TreeRightSideValues *TreePropertyWidget::updateRightSideFromUi(TreeRightSideValues* _rightSide) const
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
    _rightSide->setValues(values);
    return _rightSide;
}

void TreePropertyWidget::clear()
{
    for (QtProperty* prop : m_variantManager->properties())
        m_variantManager->setValue(prop, QVariant(0.0d));
}

void TreePropertyWidget::fillLeftSide()
{
    Q_ASSERT(m_leftSide);
    const QList<PropertyNode *> nodes = m_leftSide->nodes();
    for(PropertyNode* node : nodes)
        addProperty(toProperty(node));
}

QtProperty* TreePropertyWidget::toProperty(PropertyNode *_node)
{
    QtProperty *property = m_variantManager->addProperty(nodeType(_node), _node->description());
    m_planeProperties[_node->key()] = property;

    for(PropertyNode* node : _node->children())
    {
        QtProperty* subProperty = toProperty(node);
        property->addSubProperty(subProperty);
    }

    return property;
}

int TreePropertyWidget::nodeType(const PropertyNode *_node) const
{
    bool hasChildren = !_node->children().isEmpty();
    if(hasChildren)
        return QtVariantPropertyManager::groupTypeId();
    else
        return QVariant::Double;
}

void TreePropertyWidget::setEditable(bool _set)
{
    for(QtProperty* prop : m_variantManager->properties())
        prop->setEnabled(_set);
}

void TreePropertyWidget::setPrecision(int _newPrecision)
{
    m_precision = _newPrecision;
    for(QtProperty* prop : m_variantManager->properties())
    {
        m_variantManager->setAttribute(prop, "decimals", QVariant(_newPrecision));
        emit m_variantManager->propertyChanged(prop);
    }
    update();
}

void TreePropertyWidget::setMaxValue(int _max)
{
    m_maxValue = _max;
    for(QtProperty* prop : m_variantManager->properties())
    {
        m_variantManager->setAttribute(prop, "maximum", QVariant(_max));
        emit m_variantManager->propertyChanged(prop);
    }
    update();
}

void TreePropertyWidget::setMinValue(int _min)
{
    m_minValue = _min;
    for(QtProperty* prop : m_variantManager->properties())
    {
        m_variantManager->setAttribute(prop, "_min", QVariant(_min));
        emit m_variantManager->propertyChanged(prop);
    }
    update();
}
