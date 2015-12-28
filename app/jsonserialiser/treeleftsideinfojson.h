#ifndef TREELEFTSIDEINFOJSON_H
#define TREELEFTSIDEINFOJSON_H

#include "propertynodejson.h"
#include "../nodesinfo/treeleftsideinfo.h"

class TreeLeftSideInfoJson : public TreeLeftSideInfo
{
public:
    TreeLeftSideInfoJson();
    ~TreeLeftSideInfoJson();
    void clear() override;

    void open(const QString& _treeName) override;
    bool save() const;
    QString treeName() const override;

    const QList<PropertyNode*> nodes() override;
    QStringList planeDescriptions() const override;
    QStringList planeKeys() const override;

    int savedRightSidesCount() const override;
    QStringList savedRightSidesIds() const override;
    QString savedAverageRightSideTreeName() const override;
    TreeRightSideValues* createRightSide() const override;

    bool import(TreeLeftSideInfo* _otherInfo, ImportPolicy _policy = ResetOldData) override;

private:
    static QString extension();
    void read(const QJsonObject &_json);
    void write(QJsonObject &json) const;

private:
    QString m_treeName;

    QList<PropertyNodeJson*> m_nodes;
    QStringList m_planeDescriptions;
    QStringList m_planeKeys;

};

#endif // TREELEFTSIDEINFOJSON_H
