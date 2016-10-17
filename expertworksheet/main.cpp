#include "memory"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QShortcut>
#include <QDebug>

#include "serialization/projectsloader.h"
#include "propertytreeviewer.h"
#include "projectchooser.h"

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

void showCreateViewer(const ProjectsLoader& _loader)
{
    static std::shared_ptr<PropertyTreeViewer> viewer = std::make_shared<PropertyTreeViewer>
            (_loader.getSelf(), "metodicJudges");
    viewer->setWindowTitle("Опросник эксперта");
}

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(myMessageOutput);
#endif
    QApplication a(argc, argv);

    ProjectsLoader loader;

    ProjectChooser projectChoose(loader.getSelf(), true);
    projectChoose.hideAddButton();
    projectChoose.show();

    bool connected = QObject::connect(&projectChoose, &ProjectChooser::projectChoosen, [&]() {
        showCreateViewer(loader);
    } );

    Q_ASSERT(connected);
    return a.exec();
}
