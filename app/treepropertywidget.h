#ifndef TREEPROPERTYWIDGET_H
#define TREEPROPERTYWIDGET_H

#include <QtTreePropertyBrowser>
class QtVariantPropertyManager;

class ProperyNode;
class TreeLeftSideInfo;

class TreePropertyWidget : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    TreePropertyWidget(const QString& _propertiesFile, QWidget *_parent = 0);
    static QString averageJudgeName();

    void fillMetadata(const QString& _fileName);
    void setCurrentJudge(const QString& _name, bool _normalise);
    QString currentJudge();

    QList<double> values();
    QList<double> values(const QString& _judgeName);
    void valuesToFile(const QString& _judgeName, const QString& _file);

    void setValues(const QString& _judgeName, const QList<double>& _values);
    void readValues(const QString& _judgeName, const QString& _file);

    void setAverageCalculation(bool _normalise);
    void normalise(bool _norm);

private:
    typedef QMap<QtProperty*, QVariant> Judge;
    Judge emptyJudge();

    static TreeLeftSideInfo* getTreeLedtSideInfo(const QString &_fileName);

    void storeJudge(const QString& _name);
    void displayJudge(const QString& _name, bool _normalise);
    void displayJudge(const Judge& _judge);

    QtProperty *toProperty(ProperyNode* _node);
    int nodeType(const ProperyNode* _node) const;

    double toDouble(const QString& _str) const;
    double toDouble(const QVariant& _var) const;

    TreePropertyWidget::Judge normalise(const Judge& _judge) const;
    void setEditable(bool _set);

private:
    bool m_normalised;
    TreeLeftSideInfo* m_metaInfo;
    QtVariantPropertyManager *m_variantManager;

    QString m_currentJudgeName;
    QMap<QString, Judge> m_sourceJudges;
};

#endif // TREEPROPERTYWIDGET_H
