#include "modechooserdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModeChooserDialog dlg;
    dlg.exec();

    return a.exec();
}
