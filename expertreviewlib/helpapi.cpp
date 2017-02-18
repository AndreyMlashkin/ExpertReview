#include <QMessageBox>

#include "helpapi.h"

HelpApi::HelpApi()
{

}

void callHelp(const QString &_text)
{
    const static QString autorInfo =
                   "\n===================================\n"
                   "Связаться с авторами можно по адресам:\n"
                   "malashkin.andrey@gmail.com\n"
                   "malashkin_a_v@mail.ru";

    QMessageBox helpDialog;
    helpDialog.setWindowTitle("Справка");
    helpDialog.setText(_text + autorInfo);
    helpDialog.exec();
}
