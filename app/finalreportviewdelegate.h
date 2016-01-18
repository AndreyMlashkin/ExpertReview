#ifndef FINALREPORTVIEWDELEGATE_H
#define FINALREPORTVIEWDELEGATE_H

#include <QStyledItemDelegate>

class FinalReportViewDelegate : public QStyledItemDelegate
{
public:
    FinalReportViewDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    const QColor& colorForValue(double value) const;
};

#endif // FINALREPORTVIEWDELEGATE_H
