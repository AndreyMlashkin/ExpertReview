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
//#include <QTableView>
//#include "fulltreetablemodel.h"
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
    projectChoose.hideAddButton();
    projectChoose.show();

    ModeChooser chooser(loader.getSelf());
    chooser.setWindowTitle("Опросник эксперта");
    bool connected = QObject::connect(&projectChoose, &ProjectChooser::projectChoosen, &chooser, &QWidget::show);
    Q_ASSERT(connected);
/*
    // Test code:
    QTableView view;
    FullTreeTableModel model(loader.getSelf());
    view.setModel(&model);
    view.show();
//    QObject::connect(&projectChoose, &ProjectChooser::projectChoosen,
//                     &model,         &FullTreeTableModel::update);

    // Test code end

    auto printLoadedStructure = [&loader]()
    {
        qDebug() << loader.loadedStructure();
    };

    QShortcut *shortcutPrintStructure = new QShortcut(QKeySequence("Ctrl+P"), &chooser);
    QObject::connect(shortcutPrintStructure, &QShortcut::activated, printLoadedStructure);

    auto updateView = [&]()
    {
        qDebug() << Q_FUNC_INFO;
        model.setTreeName("test");
    };
    QShortcut *shortcutUpdateView = new QShortcut(QKeySequence("Ctrl+U"), &chooser);
    QObject::connect(shortcutUpdateView, &QShortcut::activated, updateView);
*/
    return a.exec();
}
