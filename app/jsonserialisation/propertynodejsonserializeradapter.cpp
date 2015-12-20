#include <QVariant>
#include <include/qjson/qobjecthelper.h>
#include "propertynodejsonserializeradapter.h"

PropertyNodeJsonSerializerAdapter::PropertyNodeJsonSerializerAdapter()
{
}

PropertyNodeJsonSerializerAdapter::~PropertyNodeJsonSerializerAdapter()
{
    delete m_original;
    qDeleteAll(m_jsonChildren);
    qDeleteAll(m_originalChildren);
}

QString PropertyNodeJsonSerializerAdapter::description() const
{
    return m_description;
}

QString PropertyNodeJsonSerializerAdapter::key() const
{
    return m_key;
}

QVariantList PropertyNodeJsonSerializerAdapter::children() const
{
    QVariantList ans;
    foreach (PropertyNodeJsonSerializerAdapter* child, m_jsonChildren)
    {
        ans << QJson::QObjectHelper::qobject2qvariant(child);
    }
    return ans;
}

void PropertyNodeJsonSerializerAdapter::setDescription(const QString &_description)
{
    m_description = _description;
}

void PropertyNodeJsonSerializerAdapter::setKey(const QString &_key)
{
    m_key = _key;
}

void PropertyNodeJsonSerializerAdapter::setChildren(const QVariantList &_children)
{
    foreach (const QVariant& var, _children)
    {
        PropertyNodeJsonSerializerAdapter* jsonChild = new PropertyNodeJsonSerializerAdapter();
        QJson::QObjectHelper::qvariant2qobject(var.toMap(), jsonChild);

        m_jsonChildren << jsonChild;
        m_originalChildren << jsonChild->original();
    }
}

ProperyNode *PropertyNodeJsonSerializerAdapter::original()
{
    if(!m_original)
    {
        Q_ASSERT(!m_description.isEmpty() && !m_key.isEmpty());
        m_original = new ProperyNode(m_description, m_key);
    }
    return m_original;
}
