#ifndef TREELEFTSIDEINFOFILE_H
#define TREELEFTSIDEINFOFILE_H

#include <QStringList>
#include "treeleftsideinfo.h"

class TreeLeftSideInfoFile : public TreeLeftSideInfo
{
public:
    TreeLeftSideInfoFile();
    ~TreeLeftSideInfoFile();

    QString id() const;

    const QList<ProperyNode*> nodes() override;
    QStringList planeNodes() override;

    void open(const QString& _treeName) override;

private:
    int calculateEnclosure(QString& _str);
    void clearNodes();

private:
    QString m_openedFile;
    QList<ProperyNode*> m_nodes;
    QStringList m_planeNodes;
};

#endif // TREELEFTSIDEINFOFILE_H
