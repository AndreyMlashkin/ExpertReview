#ifndef TREELEFTSIDEINFOFILE_H
#define TREELEFTSIDEINFOFILE_H

#include <QStringList>
#include "treeleftsideinfo.h"

class TreeLeftSideInfoFile : public TreeLeftSideInfo
{
public:
    TreeLeftSideInfoFile(const QString& _treeName);
    ~TreeLeftSideInfoFile();

    void open(const QString& _treeName) override;
    QString id() const override;

    const QList<ProperyNode*> nodes() override;
    QStringList planeDescriptions() const override;
    QStringList planeKeys() const override;

    int savedRightSidesCount() const override;
    QStringList savedRightSidesIds() const override;
    QString savedAverageRightSideId() const override;
    TreeRightSideValues* createRightSide() const override;

private:
    int calculateEnclosure(QString& _str);
    void clearNodes();

private:
    QString m_openedFile;
    QList<ProperyNode*> m_nodes;
    QStringList m_planeNodes;
    QStringList m_planeKeys;
};

#endif // TREELEFTSIDEINFOFILE_H
