#ifndef TREEPROPERTYWIDGET_H
#define TREEPROPERTYWIDGET_H

#include <QtTreePropertyBrowser>
class QtVariantPropertyManager;

class ProperyNode;
class TreeLeftSideInfo;
class TreeRightSideValues;
class TreeInfoFactory;

class TreePropertyWidget : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    TreePropertyWidget(TreeLeftSideInfo *_info, TreeInfoFactory* _factory, QWidget *_parent = 0);

    void setValues(TreeRightSideValues* _values);
    TreeRightSideValues* getValues() const;

    void clear();
    void setEditable(bool _set);

    void setPrecision(int _newPrecision);

private:
    void fillLeftSide();

    QtProperty *toProperty(ProperyNode* _node);
    int nodeType(const ProperyNode* _node) const;

    double toDouble(const QString& _str) const;
    double toDouble(const QVariant& _var) const;

private:
    TreeLeftSideInfo* m_info;
    TreeInfoFactory* m_factory;

    bool m_normalised;
    int m_precision;
    QtVariantPropertyManager *m_variantManager;
};

#endif // TREEPROPERTYWIDGET_H
