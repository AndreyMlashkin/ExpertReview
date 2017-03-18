#include <QDebug>
#include <QPainter>
#include "finalreportviewdelegate.h"

FinalReportViewDelegate::FinalReportViewDelegate()
    : QStyledItemDelegate()
{
}

QString FinalReportViewDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    bool ok;
    double doubleValue = value.toDouble(&ok);
    if(!ok)
        return QStyledItemDelegate::displayText(value, locale);
    return QString::number(doubleValue * 100.0);
}

void FinalReportViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*bool isOk;
    double val = index.data().toDouble(&isOk);
    Q_ASSERT(isOk);

    painter->fillRect(option.rect, colorForValue(val));
    */
    QStyledItemDelegate::paint(painter, option, index);
}

const QColor &FinalReportViewDelegate::colorForValue(double value) const
{
    static QColor increaseColor(Qt::green);
    static QColor decreaseColor(Qt::red);
    static QColor nullColor(Qt::transparent);

    if(value > 0)
        return increaseColor;
    if(value < 0)
        return decreaseColor;
    return nullColor;
}
