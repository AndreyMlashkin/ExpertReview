#include "memory"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QShortcut>
#include <QDebug>

#include "serialization/projectsloader.h"
#include "serialization/nodesinfo/treerightsidevalues.h"
#include "propertytreeviewer.h"
#include "projectchooser.h"

QString leftSideName("metodicJudges");

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

void showTreeViewer(ProjectsLoader& _loader, const QString& _expertName)
{
    Q_ASSERT(_loader.avaliableLeftSides().contains(leftSideName));

    QStringList rSides = _loader.avaliableRightSides(leftSideName);
    for(const QString& rSide : rSides)
    {
        if(rSide != _expertName)
        {
            _loader.removeRightSide(rSide);
        }
    }
    TreeRightSideValues* rSide =
            _loader.getOrCreateRightSide(leftSideName, _expertName);
    rSide->setGuiName(_expertName);

    new PropertyTreeViewer(_loader.getSelf(), QString(leftSideName),
                           PropertyTreeViewer::Minimal);
}

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(myMessageOutput);
#endif
    QApplication a(argc, argv);

    if(argc > 1)
    {
        leftSideName = QString(argv[1]);
    }

    ProjectsLoader loader;

    ProjectChooser projectChooser(loader.getSelf(), true);
    projectChooser.hideAddButton();
    projectChooser.show();

    bool connected = QObject::connect(&projectChooser, &ProjectChooser::projectChoosen, [&]() {
        showTreeViewer(loader, projectChooser.getExpertName());
    } );

    Q_ASSERT(connected);
    return a.exec();
}
