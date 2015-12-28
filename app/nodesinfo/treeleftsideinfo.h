#ifndef TREELEFTSIDEINFO_H
#define TREELEFTSIDEINFO_H

#include <QStringList>
#include <QJsonObject>

class PropertyNode;
class TreeRightSideValues;

class TreeLeftSideInfo
{
public:
    TreeLeftSideInfo() {}
    virtual ~TreeLeftSideInfo() {}
    virtual void clear() {} // !!! todo abstract

    virtual void open(const QString& _treeName) = 0;
    virtual QString treeName() const = 0;

    virtual const QList<PropertyNode*> nodes() = 0;
    virtual QStringList planeDescriptions() const = 0;
    virtual QStringList planeKeys() const = 0;

    QStringList titles() const;
    QStringList leafs() const;

    virtual int savedRightSidesCount() const = 0;
    virtual QStringList savedRightSidesTreeNames() const = 0;
    virtual QString savedAverageRightSideTreeName() const = 0;
    virtual TreeRightSideValues* createRightSide() const = 0;
    TreeRightSideValues* openRightSide(int _num);

    enum ImportPolicy { AddData, ResetOldData };
    virtual bool import(TreeLeftSideInfo* _otherInfo, ImportPolicy _policy = ResetOldData);

private:
    void fillTitlesAndLeafs();

private:
    mutable QStringList m_titles;
    mutable QStringList m_leafs;
};

#endif // TREELEFTSIDEINFO_H
