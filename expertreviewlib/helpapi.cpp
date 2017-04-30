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
                   "Малашкин Андрей Анатольевич\t\tmalashkin.andrey@gmail.com\n"
                   "Малашкин Анатолий Владимирович\tmalashkin_a_v@mail.ru\n"
                   "Морозова Лариса Эдуардовна\t\t lora.16m@gmail.com";

    QMessageBox helpDialog;
    helpDialog.setWindowTitle("Справка");
    helpDialog.setText(_text + autorInfo);
    helpDialog.exec();
}
