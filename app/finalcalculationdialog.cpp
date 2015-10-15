#include "finalcalculationdialog.h"
#include "ui_finalcalculationdialog.h"

FinalCalculationDialog::FinalCalculationDialog(double _firstProjValue, double _secondProjValue, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::FinalCalculationDialog)
{
    m_ui->setupUi(this);
    QString report = m_ui->label->text();
    report = report.arg(QString::number(_firstProjValue),
                        QString::number(_secondProjValue));

    m_ui->label->setText(report);

    connect(m_ui->okButton, SIGNAL(clicked()), SLOT(accept()));
}

FinalCalculationDialog::~FinalCalculationDialog()
{
    delete m_ui;
}
