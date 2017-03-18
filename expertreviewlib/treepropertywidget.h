#ifndef TREEPROPERTYWIDGET_H
#define TREEPROPERTYWIDGET_H

#include <QMap>
#include <include/qtpropertybrowser/QtTreePropertyBrowser>
class QtVariantPropertyManager;

class PropertyNode;
class TreeLeftSideInfo;
class TreeRightSideValues;

//!
//! \brief The TreePropertyWidget class Класс графического интерфейса, отображающий значения
//! одного проекта.
//!
class TreePropertyWidget : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    TreePropertyWidget(TreeLeftSideInfo *_info, QWidget *_parent = 0);
    ~TreePropertyWidget();

    //! \brief setValues присваивает значения проекта
    void setValues(TreeRightSideValues* _values);

    //! \brief updateRightSideFromUi возвращает заполненные значения одного проекта
    //! из графического интерфейса.
    TreeRightSideValues* updateRightSideFromUi(TreeRightSideValues *_rightSide) const;

    //! \brief clear сбрасывает все значения в 0
    void clear();
    //! \brief setEditable сделать виджет редактируемым.
    void setEditable(bool _set);

    //! \brief setPrecision установить новую точность отображения знаков после запятой.
    void setPrecision(int _newPrecision);

    void setMaxValue (int _max);
    void setMinValue (int _min);

private:
    void fillLeftSide();
    QtProperty *toProperty(PropertyNode* _node);
    int nodeType(const PropertyNode* _node) const;

//    double toDouble(const QString& _str) const;
//    double toDouble(const QVariant& _var) const;

private:
    QMap<QString, QtProperty*> m_planeProperties;
    TreeLeftSideInfo* m_leftSide;

    bool m_normalised;

    int m_precision;
    int m_minValue;
    int m_maxValue;
    QtVariantPropertyManager *m_variantManager;
};

#endif // TREEPROPERTYWIDGET_H
