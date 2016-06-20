#include <QFile>
#include <QTextStream>

#include "finalcalculationdialog.h"
#include "ui_finalcalculationdialog.h"
#include "fulltreetablemodel.h"

FinalCalculationDialog::FinalCalculationDialog(const ProjectsLoaderPtr &_loader, double _firstProjValue, double _secondProjValue, const QString& _treeName, QWidget *parent)
    : QDialog(parent),
      m_ui(new Ui::FinalCalculationDialog),
      m_loader(_loader),
      m_model(new FullTreeTableModel(_treeName, _loader))
{
    m_ui->setupUi(this);
    QString report = m_ui->label->text();
    report = report.arg(QString::number(_firstProjValue),
                        QString::number(_secondProjValue));

    m_ui->label->setText(report);

    connect(m_ui->okButton, SIGNAL(clicked()), SLOT(accept()));

    m_model->normaliseRows();
    m_model->setBaseName("Проект");
    writeReport();

    m_ui->table->setModel(m_model);
}

FinalCalculationDialog::~FinalCalculationDialog()
{
    delete m_ui;
}

void FinalCalculationDialog::resizeEvent(QResizeEvent*)
{
    int totalWidth = m_ui->table->width();
    totalWidth = totalWidth - m_ui->table->verticalHeader()->width() - 10; // !!!

    m_ui->table->setColumnWidth(0, totalWidth / 2);
    m_ui->table->setColumnWidth(1, totalWidth / 2);
}

void FinalCalculationDialog::writeReport() const
{
    QFile file("report.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << m_model->report();
}
