#ifndef PROPERTYNODEJSONSERIALIZERADAPTER_H
#define PROPERTYNODEJSONSERIALIZERADAPTER_H

#include <QObject>
#include <properynode.h>

class PropertyNodeJsonSerializerAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nodes READ description WRITE setDescription)
    Q_PROPERTY(QString key   READ key         WRITE setKey)


public:
    PropertyNodeJsonSerializerAdapter();
    ~PropertyNodeJsonSerializerAdapter();

    QString description() const;
    QString key() const;
    QVariantList children() const;

    void setDescription(const QString& _description);
    void setKey(const QString& _key);
    void setChildren(const QVariantList& _children);

    ProperyNode* original();

private:
    ProperyNode* m_original = nullptr;
    QString m_description;
    QString m_key;

    QList<PropertyNodeJsonSerializerAdapter*> m_jsonChildren;
    QList<ProperyNode*> m_originalChildren;
};

#endif // PROPERTYNODEJSONSERIALIZERADAPTER_H
