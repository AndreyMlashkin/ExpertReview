#ifndef TREELEFTSIDEINFO_H
#define TREELEFTSIDEINFO_H

#include <QStringList>

class ProperyNode;
class TreeRightSideValues;

class TreeLeftSideInfo
{
public:
    TreeLeftSideInfo() {}
    virtual ~TreeLeftSideInfo() {}

    virtual void open(const QString& _treeName) = 0;
    virtual QString id() const = 0;

    virtual const QList<ProperyNode*> nodes() = 0;
    virtual QStringList planeDescriptions() const = 0;
    virtual QStringList planeKeys() const = 0;

    QStringList titles() const;
    QStringList leafs() const;

    virtual int savedRightSidesCount() const = 0;
    virtual QStringList savedRightSidesIds() const = 0;
    virtual QString savedAverageRightSideId() const = 0;
    virtual TreeRightSideValues* createRightSide() const = 0;
    TreeRightSideValues* openRightSide(int _num);

    virtual void import(TreeLeftSideInfo* _info);

private:
    void fillTitlesAndLeafs();

private:
    mutable QStringList m_titles;
    mutable QStringList m_leafs;
};

#endif // TREELEFTSIDEINFO_H
