#ifndef TREELEFTSIDEINFO_H
#define TREELEFTSIDEINFO_H

#include <QList>
#include <QString>

class ProperyNode;
class TreeLeftSideInfo
{
public:
    TreeLeftSideInfo() {}
    virtual ~TreeLeftSideInfo() {}

    virtual QString id() const = 0;

    virtual const QList<ProperyNode*> nodes() = 0;
    virtual QStringList planeNodes() = 0;

    virtual void open(const QString& _treeName) = 0;
};

#endif // TREELEFTSIDEINFO_H
