#include "memory"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QShortcut>
#include <QDebug>

#include "serialization/projectsloader.h"
#include "projectchooser.h"
#include "modechooser.h"

// TODO delete
#include <QTableView>
#include "fulltreetablemodel.h"
// TODO delete

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
    projectChoose.show();

    ModeChooser chooser(loader.getSelf());
    QObject::connect(&projectChoose, &ProjectChooser::projectChoosen, &chooser, &QWidget::show);

    // Test code:
//    QTableView view;
//    FullTreeTableModel model(loader.getSelf());
//    view.setModel(&model);
//    model.setTreeName("metodicJudges");
//    view.show();
//    QObject::connect(&projectChoose, &ProjectChooseDialog::projectChoosen,
//                     &model,         &FullTreeTableModel::update);

    // Test code end

    auto printLoadedStructure = [&loader]()
    {
        qDebug() << loader.loadedStructure();
    };

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+P"), &chooser);
    QObject::connect(shortcut, &QShortcut::activated, printLoadedStructure);

    return a.exec();
}
