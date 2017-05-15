#include "memory"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QShortcut>
#include <QDebug>

#include "serialization/projectsloader.h"
#include "projectchooser.h"
#include "modechooser.h"

#include "norwegianwoodstyle.h"

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

    ProjectsLoader loader;

    ProjectChooser projectChoose(loader.getSelf());
    projectChoose.resize(800,600);
    projectChoose.showMaximized();

    ModeChooser chooser(loader.getSelf());
    bool connected = QObject::connect(&projectChoose, &ProjectChooser::projectChoosen, &chooser, &QWidget::showMaximized);
    Q_ASSERT(connected);

    QObject::connect(&projectChoose, &ProjectChooser::projectChoosen,
    [&]()
    {
        loader.loadAll();
    }  );

    QApplication::setStyle(new NorwegianWoodStyle);

    return a.exec();
}
