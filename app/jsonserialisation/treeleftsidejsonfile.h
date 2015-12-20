#ifndef TREELEFTSIDEJSONFILE_H
#define TREELEFTSIDEJSONFILE_H

#include <QObject>
#include <QStringList>
#include <QVariantList>

#include <nodesinfo/treeleftsideinfo.h>

class TreeLeftSideJsonFile : public QObject, public TreeLeftSideInfo
{
    Q_OBJECT
    Q_PROPERTY(QVariantList nodes READ serialiseNodes WRITE deseraliseNodes)

public:
    TreeLeftSideJsonFile();
    ~TreeLeftSideJsonFile();

    void open(const QString& _treeName) override;
    QString id() const override;

    const QList<ProperyNode*> nodes() override;
    QStringList planeDescriptions() const override;
    QStringList planeKeys() const override;

    int savedRightSidesCount() const override;
    QStringList savedRightSidesIds() const override;
    QString savedAverageRightSideId() const override;
    TreeRightSideValues* createRightSide() const override;

    QVariantList serialiseNodes() const;
    void deseraliseNodes(const QVariantList& _nodes);

//private:
//    int calculateEnclosure(QString& _str);
//    void clearNodes();

private:
    QString m_openedFile;
    QList<ProperyNode*> m_nodes;
    QStringList m_planeNodes;
    QStringList m_planeKeys;


};

#endif // TREELEFTSIDEJSONFILE_H
