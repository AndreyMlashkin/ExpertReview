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

    void fillMetadata();
    void setCurrentJudge(const QString _name);
    QString currentJudge();

private:
    QtProperty *toProperty(ProperyNode* _node);
    int nodeType(const ProperyNode* _node) const;

    double toDouble(const QString& _str);
    double toDouble(const QVariant& _var);

private:
    QtVariantPropertyManager *m_variantManager;

    QString m_currentJudgeName;

    typedef QMap<QtProperty*, QVariant> Judge;

    QMap<QString, Judge> m_associatedValues;
//    QMap<QtProperty*, QVariant> m_associatedValues;
};

#endif // TREEPROPERTYWIDGET_H
