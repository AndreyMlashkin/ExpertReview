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

    fill();
}

void TreePropertyWidget::fill()
{
    TreeMetaInfo metaInfo;
    const QList<ProperyNode *> nodes = metaInfo.nodes();

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
