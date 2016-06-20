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
    foreach (QtProperty* prop, m_variantManager->properties())
        m_variantManager->setValue(prop, QVariant(0.0d));
}

void TreePropertyWidget::fillLeftSide()
{
    const QList<PropertyNode *> nodes = m_leftSide->nodes();
    foreach(PropertyNode* node, nodes)
        addProperty(toProperty(node));
}

QtProperty* TreePropertyWidget::toProperty(PropertyNode *_node)
{
    QtProperty *property = m_variantManager->addProperty(nodeType(_node), _node->description());
    m_planeProperties[_node->key()] = property;

    foreach(PropertyNode* node, _node->children())
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
