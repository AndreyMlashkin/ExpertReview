#include <QLocale>

#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <QtTreePropertyBrowser>

#include "treepropertywidget.h"

#include "nodesinfo/treeleftsideinfofile.h"
#include "nodesinfo/treerightsidevaluesfile.h"
#include "nodesinfo/treeinfofactory.h"
#include "properynode.h"

TreePropertyWidget::TreePropertyWidget(TreeLeftSideInfo* _info, TreeInfoFactory *_factory, QWidget *_parent)
    : QtTreePropertyBrowser(_parent),
      m_info(_info),
      m_factory(_factory),
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

    QStringList orderedKeys = m_info->planeNodes();

    int size = qMin(_values->values().size(), orderedKeys.size());
    for(int i = 0; i < size; ++i)
    {
        QString key = orderedKeys.at(i);
        QVariant newVal = _values->values().at(i);

        QtProperty* prop = findPropery(m_variantManager, key);
        m_variantManager->setAttribute(prop, "decimals", QVariant(m_precision));
        m_variantManager->setValue(prop, newVal);
    }
}

TreeRightSideValues *TreePropertyWidget::getValues() const
{
    QVariantList values;
    QStringList orderedKeys = m_info->planeNodes();
    foreach (QString key, orderedKeys)
    {
        QtProperty* prop = findPropery(m_variantManager, key);
        values << m_variantManager->value(prop);
    }

    TreeRightSideValues* ans = m_factory->getRightSideValues();
    ans->setValues(values);
    return ans;
}

void TreePropertyWidget::clear()
{
    foreach (QtProperty* prop, m_variantManager->properties())
        m_variantManager->setValue(prop, QVariant(0.0d));
}

void TreePropertyWidget::fillLeftSide()
{
    const QList<ProperyNode *> nodes = m_info->nodes();
    foreach(ProperyNode* node, nodes)
        addProperty(toProperty(node));
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

double TreePropertyWidget::toDouble(const QString &_str) const
{
    if(_str.isEmpty())
        return 0;

    bool isOk;
    double ans = QLocale::system().toDouble(_str, &isOk);
    if(!isOk)
        ans = _str.toDouble(&isOk);
    Q_ASSERT(isOk);

    return ans;
}

double TreePropertyWidget::toDouble(const QVariant &_var) const
{
    return toDouble(_var.toString());
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
