#include "memory"

#include <QApplication>
#include <QFile>
#include <QTextStream>

#include "serialization/projectsloader.h"
#include "projectchoosedialog.h"
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

    ProjectChooseDialog projectChoose(loader.getSelf());
    projectChoose.show();

    ModeChooser chooser(loader.getSelf());
    QObject::connect(&projectChoose, &ProjectChooseDialog::projectChoosen, &chooser, &QWidget::show);

    // Test code:
//    QTableView view;
//    FullTreeTableModel model(loader.getSelf());
//    view.setModel(&model);
//    model.setTreeName("metodicJudges");
//    view.show();
//    QObject::connect(&projectChoose, &ProjectChooseDialog::projectChoosen,
//                     &model,         &FullTreeTableModel::update);

    // Test code end

    return a.exec();
}
