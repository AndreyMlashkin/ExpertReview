#ifndef FINALCALCULATIONDIALOG_H
#define FINALCALCULATIONDIALOG_H

#include <QDialog>
#include "serialization/projectsloader.h"

namespace Ui
{
class FinalCalculationDialog;
}

class FullTreeTableModel;

class FinalCalculationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FinalCalculationDialog(const ProjectsLoaderPtr& _loader,
                                    double _firstProjValue,
                                    double _secondProjValue,
                                    const QString& _treeName, QWidget *parent = 0);
    ~FinalCalculationDialog();

private:
    void resizeEvent(QResizeEvent *_ev);
    void writeReport() const;

private:
    Ui::FinalCalculationDialog *m_ui;
    ProjectsLoaderPtr m_loader;
    FullTreeTableModel* m_model;
};

#endif // FINALCALCULATIONDIALOG_H
