#ifndef MODECHOOSERDIALOG_H
#define MODECHOOSERDIALOG_H

#include <QDialog>

namespace Ui {
class ModeChooserDialog;
}

class ModeChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModeChooserDialog(QWidget *parent = 0);
    ~ModeChooserDialog();

private slots:
    void callMetodicJudges();

private:
    Ui::ModeChooserDialog *ui;
};

#endif // MODECHOOSERDIALOG_H
