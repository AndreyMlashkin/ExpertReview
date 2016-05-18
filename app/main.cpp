#include "memory"

#include <QApplication>
#include <QFile>
#include <QTextStream>

#include "serialization/projectsloader.h"
#include "modechooser.h"
#include "projectchoosedialog.h"

void myMessageOutput(QtMsgType, const QMessageLogContext&, const QString &msg)
{
    static QTextStream out;
    static QFile file("log.txt");
    if(!file.isOpen())
    {
        file.open(QIODevice::WriteOnly);
        out.setDevice(&file);
    }
    out << msg << endl;
}

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(myMessageOutput);
#endif
    QApplication a(argc, argv);

    std::shared_ptr<ProjectsLoader> loader = std::make_shared<ProjectsLoader>();

    ProjectChooseDialog projectChoose(loader);
    projectChoose.show();

    ModeChooser chooser(loader);

    QObject::connect(&projectChoose, &ProjectChooseDialog::projectChoosen, &chooser, &QWidget::show);

    return a.exec();
}
