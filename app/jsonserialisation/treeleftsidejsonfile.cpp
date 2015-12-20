#include <QDebug>
#include <QFile>

#include <include/qjson/parser.h>
#include <include/qjson/qobjecthelper.h>
#include <include/qjson/serializer.h>

#include "treeleftsidejsonfile.h"
#include "properynode.h"
#include "propertynodejsonserializeradapter.h"

#include "../nodesinfo/treeleftsideinfofile.h"

TreeLeftSideJsonFile::TreeLeftSideJsonFile()
    : QObject(),
      TreeLeftSideInfo()
{
}

TreeLeftSideJsonFile::~TreeLeftSideJsonFile()
{
    QVariantMap self = QJson::QObjectHelper::qobject2qvariant(this);

    QFile file(m_openedFile);
    bool successOpen = (!file.open(QIODevice::WriteOnly | QIODevice::Text));
    Q_ASSERT(successOpen);

    QJson::Serializer ser;
    QByteArray rawData = ser.serialize(self);
    file.write(rawData);

    qDeleteAll(m_jsonNodes);
}

void TreeLeftSideJsonFile::open(const QString &_treeName)
{
    m_openedFile = _treeName;

    QFile file(_treeName);
    bool isOk = false;
    isOk = file.open(QIODevice::ReadOnly);
    if(!isOk)
    {
        qDebug() << "file not found" << _treeName;
        return;
    }

    QJson::Parser par;
    QVariantMap var = par.parse(&file, &isOk).toMap();
    if(!isOk)
    {
        qDebug() << "file cannot be parsed. wrong format.";
        return;
    }
    QJson::QObjectHelper::qvariant2qobject(var, this);
}

QString TreeLeftSideJsonFile::id() const
{
    return m_openedFile;
}

const QList<ProperyNode *> TreeLeftSideJsonFile::nodes()
{
    return m_nodes;
}

QStringList TreeLeftSideJsonFile::planeDescriptions() const
{
}

QStringList TreeLeftSideJsonFile::planeKeys() const
{

}

int TreeLeftSideJsonFile::savedRightSidesCount() const
{

}

QStringList TreeLeftSideJsonFile::savedRightSidesIds() const
{

}

QString TreeLeftSideJsonFile::savedAverageRightSideId() const
{

}

TreeRightSideValues *TreeLeftSideJsonFile::createRightSide() const
{
    return nullptr;
}

QVariantList TreeLeftSideJsonFile::serialiseNodes() const
{
    QVariantList ans;
    foreach(PropertyNodeJsonSerializerAdapter* node, m_jsonNodes)
    {
        ans << QJson::QObjectHelper::qobject2qvariant(node);
    }
    return ans;
}

void TreeLeftSideJsonFile::deseraliseNodes(const QVariantList &_nodes)
{
    qDeleteAll(m_jsonNodes);
    foreach (const QVariant& var, _nodes)
    {
        PropertyNodeJsonSerializerAdapter* jsonNode = new PropertyNodeJsonSerializerAdapter();
        QJson::QObjectHelper::qvariant2qobject(var.toMap(), jsonNode);

        m_jsonNodes << jsonNode;
        m_nodes     << jsonNode->original();
    }
}
