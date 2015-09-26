//#include "mainwindow.h"
#include "modechooserdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    ModeChooserDialog dlg;
    dlg.exec();

    return a.exec();
}
