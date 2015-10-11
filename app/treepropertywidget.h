#ifndef TREEPROPERTYWIDGET_H
#define TREEPROPERTYWIDGET_H

#include <QtTreePropertyBrowser>
class QtVariantPropertyManager;

class ProperyNode;
class TreeLeftSideInfo;
class TreeRightSideValues;
class TreeInfoFactory;

// Переписать! вместо судей - TreeLeftSideInfo и TreeRightSideValues
class TreePropertyWidget : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    TreePropertyWidget(TreeLeftSideInfo *_info, TreeInfoFactory* _factory, QWidget *_parent = 0);

    void setValues(TreeRightSideValues* _values);
    TreeRightSideValues* getValues() const;

    void clear();
//    static QString averageJudgeName();



//    void setCurrentJudge(const QString& _name, bool _normalise);
//    QString currentJudge();

//    QList<double> values();
//    QList<double> values(const QString& _judgeName);
//    void valuesToFile(const QString& _judgeName, const QString& _file);

//    void setValues(const QString& _judgeName, const QList<double>& _values);
//    void readValues(const QString& _judgeName, const QString& _file);

//    void setAverageCalculation(bool _normalise);
//    void normalise(bool _norm);
/*
private:
    typedef QMap<QtProperty*, QVariant> Judge;
    Judge emptyJudge();
*/
    void fillLeftSide();
/*
    void storeJudge(const QString& _name);
    void displayJudge(const QString& _name, bool _normalise);
    void displayJudge(const Judge& _judge);
*/
    QtProperty *toProperty(ProperyNode* _node);
    int nodeType(const ProperyNode* _node) const;

    double toDouble(const QString& _str) const;
    double toDouble(const QVariant& _var) const;
/*
    TreePropertyWidget::Judge normalise(const Judge& _judge) const;
*/    void setEditable(bool _set);

private:
    TreeLeftSideInfo* m_info;
    TreeInfoFactory* m_factory;

    bool m_normalised;
    QtVariantPropertyManager *m_variantManager;

//    QString m_currentJudgeName;
//    QMap<QString, Judge> m_sourceJudges;


};

#endif // TREEPROPERTYWIDGET_H
