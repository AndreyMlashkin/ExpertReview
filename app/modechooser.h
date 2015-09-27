#ifndef MODECHOOSER_H
#define MODECHOOSER_H

#include <QWidget>

namespace Ui {
class ModeChooser;
}

class ModeChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ModeChooser(QWidget *parent = 0);
    ~ModeChooser();

private slots:
    void callMetodicJudges();
    void callCalculation();

private:
    Ui::ModeChooser *m_ui;
};

#endif // MODECHOOSER_H
