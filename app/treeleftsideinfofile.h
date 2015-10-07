#ifndef TREELEFTSIDEINFOFILE_H
#define TREELEFTSIDEINFOFILE_H

#include "treeleftsideinfo.h"

class TreeLeftSideInfoFile : public TreeLeftSideInfo
{
public:
    TreeLeftSideInfoFile();
    ~TreeLeftSideInfoFile();

    const QList<ProperyNode*> nodes() override;
    QStringList planeNodes() override;

    void open(const QString& _treeName) override;
    void save(const QString& _treeName) const override;

private:
    int calculateEnclosure(QString& _str);
    void clearNodes();

private:
    QString m_openedFile;
    QList<ProperyNode*> m_nodes;
    QStringList m_planeNodes;
};

#endif // TREELEFTSIDEINFOFILE_H
