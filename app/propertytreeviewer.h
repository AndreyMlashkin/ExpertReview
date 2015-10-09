#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui
{
    class PropertyTreeViewer;
}
class TreePropertyWidget;
class TreeInfoFactory;
class TreeRightSideValues;
class TreeLeftSideInfo;

// нужно сделать инъекцию зависимости TreePropertyWidget - получать в конструктре
class PropertyTreeViewer : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyTreeViewer(const QString& _treeId, QWidget *parent = 0);
    ~PropertyTreeViewer();

    void setDefaultTabName(const QString& _name);
    QString defaultTabName() const;

private slots:
    void tabChanged(int _newNum);
    void normalise(bool _toggle);

private:
    void init();
    void addTab();
    bool normalise() const;    
    QString generateTabName(int _num) const;

private:
    Ui::PropertyTreeViewer *m_ui;
    QString m_defaultTabName;
    QString m_treeId;

    TreePropertyWidget* m_treePropertyWidget;

    TreeInfoFactory* m_factory;
    TreeLeftSideInfo* m_leftInfo;
    QList<TreeRightSideValues*> m_values;
};

#endif // MAINWINDOW_H
