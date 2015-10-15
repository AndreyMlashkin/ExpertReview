#ifndef FINALCALCULATIONDIALOG_H
#define FINALCALCULATIONDIALOG_H

#include <QDialog>

namespace Ui {
class FinalCalculationDialog;
}

class FinalCalculationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FinalCalculationDialog(double _firstProjValue, double _secondProjValue, QWidget *parent = 0);
    ~FinalCalculationDialog();

private:
    Ui::FinalCalculationDialog *m_ui;
};

#endif // FINALCALCULATIONDIALOG_H
