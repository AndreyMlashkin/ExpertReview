#include "modechooser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModeChooser chooser;
    chooser.show();

    return a.exec();
}
