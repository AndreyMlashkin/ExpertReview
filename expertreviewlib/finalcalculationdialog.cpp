#include <QFile>
#include <QTextStream>

#include "finalcalculationdialog.h"
#include "ui_finalcalculationdialog.h"
#include "fulltreetablemodel.h"
#include "finalreportviewdelegate.h"

FinalCalculationDialog::FinalCalculationDialog(const ProjectsLoaderPtr &_loader, double _firstProjValue, double _secondProjValue, const QString& _treeName, QWidget *parent)
    : QDialog(parent),
      m_ui(new Ui::FinalCalculationDialog),
      m_loader(_loader),
      m_model(new FullTreeTableModel(_treeName, _loader))
{
    m_ui->setupUi(this);
    m_ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QString report = m_ui->label->text();
    report = report.arg(QString::number(_firstProjValue),
                        QString::number(_secondProjValue));

    m_ui->label->setText(report);

    m_model->normaliseRows();
    m_model->setBaseName("Проект");
    writeReport();

    m_ui->table->setModel(m_model);
    m_ui->table->setItemDelegate(new FinalReportViewDelegate());

    connect(m_ui->okButton, SIGNAL(clicked(bool)), SLOT(accept()));
}

FinalCalculationDialog::~FinalCalculationDialog()
{
    delete m_model;
    delete m_ui;
}

void FinalCalculationDialog::resizeEvent(QResizeEvent* _ev)
{
    QDialog::resizeEvent(_ev);
    int columnCount = m_model->columnCount();
    int allColunsWidth = 0;
    for(int i = 0; i < columnCount; ++i)
    {
        allColunsWidth += m_ui->table->columnWidth(i);
    }

    int fullWidth = m_ui->table->width() - allColunsWidth;
    m_ui->table->verticalHeader()->setMaximumWidth(fullWidth - 19);
}

void FinalCalculationDialog::writeReport() const
{
    QFile file("report.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << m_model->report();
}
