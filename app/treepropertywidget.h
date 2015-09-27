#ifndef TREEPROPERTYWIDGET_H
#define TREEPROPERTYWIDGET_H

#include <QtTreePropertyBrowser>
class QtVariantPropertyManager;

class ProperyNode;
class TreeMetaInfo;

class TreePropertyWidget : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    TreePropertyWidget(QWidget *_parent = 0);
    static QString averageJudgeName();

    void fillMetadata();
    void setCurrentJudge(const QString& _name);
    QString currentJudge();

    void setAverageCalculation();

private:
    typedef QMap<QtProperty*, QVariant> Judge;

    static TreeMetaInfo* getTreeMetaInfo();

    void storeJudge(const QString& _name);
    void displayJudge(const QString& _name);
    void displayJudge(const Judge& _judge);

    QtProperty *toProperty(ProperyNode* _node);
    int nodeType(const ProperyNode* _node) const;

    double toDouble(const QString& _str);
    double toDouble(const QVariant& _var);

private:
    QtVariantPropertyManager *m_variantManager;

    QString m_currentJudgeName;
    QMap<QString, Judge> m_associatedValues;
};

#endif // TREEPROPERTYWIDGET_H
