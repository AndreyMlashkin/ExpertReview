#include "modechooserdialog.h"
#include "ui_modechooserdialog.h"

#include "propertytreeviewer.h"

ModeChooserDialog::ModeChooserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModeChooserDialog)
{
    ui->setupUi(this);

    connect(ui->metodicJudges, SIGNAL(clicked()), SLOT(callMetodicJudges()));
}

ModeChooserDialog::~ModeChooserDialog()
{
    delete ui;
}

void ModeChooserDialog::callMetodicJudges()
{
    PropertyTreeViewer* metodicJudges = new PropertyTreeViewer();
    metodicJudges->show();
    hide();
}
