#ifndef TREEPROPERTYWIDGET_H
#define TREEPROPERTYWIDGET_H

#include <QtTreePropertyBrowser>
class QtVariantPropertyManager;

class ProperyNode;
class TreePropertyWidget : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    TreePropertyWidget(QWidget *_parent = 0);

    void fill();

private:
    QtProperty *toProperty(ProperyNode* _node);
    int nodeType(const ProperyNode* _node) const;

private:
    QtVariantPropertyManager *m_variantManager;
};

#endif // TREEPROPERTYWIDGET_H
