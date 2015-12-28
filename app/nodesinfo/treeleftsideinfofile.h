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
    QString treeName() const override;

    const QList<PropertyNode*> nodes() override;
    QStringList planeDescriptions() const override;
    QStringList planeKeys() const override;

    int savedRightSidesCount() const override;
    QStringList savedRightSidesTreeNames() const override;
    QString savedAverageRightSideTreeName() const override;
    TreeRightSideValues* createRightSide() const override;

private:
    int calculateEnclosure(QString& _str);
    void clearNodes();

private:
    QString m_openedFile;
    QList<PropertyNode*> m_nodes;
    QStringList m_planeDescriptions;
    QStringList m_planeKeys;
};

#endif // TREELEFTSIDEINFOFILE_H
