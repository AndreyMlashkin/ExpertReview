#ifndef TREELEFTSIDEINFOJSON_H
#define TREELEFTSIDEINFOJSON_H

#include "propertynodejson.h"
#include "../nodesinfo/treeleftsideinfo.h"

//!
//! \brief The TreeLeftSideInfoJson class класс для описания метаинформации проектов
//! в реализации json
//!
class TreeLeftSideInfoJson : public TreeLeftSideInfo
{
public:
    TreeLeftSideInfoJson(const ProjectsLoaderPtr& _loader);
    TreeLeftSideInfoJson(const QString& _treeName, const ProjectsLoaderPtr& _loader);
    ~TreeLeftSideInfoJson();
    void clear() override;

    void open(const QString& _treeName) override;
    bool save() const;
    QString treeName() const override;
    QString name() const override;
    QString getPath() const;

    const QList<PropertyNode*> nodes() override;
    QStringList planeDescriptions() const override;
    QStringList planeKeys() const override;

    QList<int> planeMinValues() const override;
    QList<int> planeMaxValues() const override;

    int savedRightSidesCount() const override;
    QString defaultRightSideTreeName() const override;
    QStringList savedRightSidesTreeNames() const override;
    QString savedAverageRightSideTreeName() const override;
    TreeRightSideValues* createRightSide() const override;

    bool import(TreeLeftSideInfo* _otherInfo, ImportPolicy _policy = ResetOldData) override;

private:
    static QStringList getPlaneListOfProperties(const QJsonObject& _json, const QString& _prop);

    QString rightSidePath(int _numer) const;

    void read(const QJsonObject &_json);
    void write(QJsonObject &json) const;
    QJsonObject& actualJson() const;      

private:
    QString m_guiName;
    QString m_treeName;
    QString m_defaultRightSideTreeName;
    mutable bool m_isActual;
    mutable QJsonObject m_actualJson;

    QList<PropertyNodeJson*> m_nodes;
};

#endif // TREELEFTSIDEINFOJSON_H
