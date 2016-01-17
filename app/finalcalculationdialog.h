#ifndef FINALCALCULATIONDIALOG_H
#define FINALCALCULATIONDIALOG_H

#include <QDialog>

namespace Ui
{
class FinalCalculationDialog;
}

class FullTreeTableModel;

class FinalCalculationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FinalCalculationDialog(double _firstProjValue, double _secondProjValue, const QString& _treeName, QWidget *parent = 0);
    ~FinalCalculationDialog();

private:
    void resizeEvent(QResizeEvent *);
    void writeReport() const;

private:
    Ui::FinalCalculationDialog *m_ui;
    FullTreeTableModel* m_model;
};

#endif // FINALCALCULATIONDIALOG_H
