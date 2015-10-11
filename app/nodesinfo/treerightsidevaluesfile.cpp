#include <QFile>
#include <QTextStream>

#include "treerightsidevaluesfile.h"

TreeRightSideValuesFile::TreeRightSideValuesFile()
{}

QVariantList TreeRightSideValuesFile::values() const
{
    return m_values;
}

void TreeRightSideValuesFile::setValues(const QVariantList &_values)
{
    m_values = _values;
}

void TreeRightSideValuesFile::readValues(const QString& _id)
{
    if(_id.isEmpty())
        return;

    m_values.clear();

    QFile file(_id);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        line = line.trimmed();
        QVariant v(line.toDouble());
        m_values << v;
    }
}

void TreeRightSideValuesFile::writeValues(const QString &_id)
{
    if(_id.isEmpty())
        return;

    QFile file(_id);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    foreach (QVariant v, m_values)
    {
        QByteArray line = v.toByteArray();
        line.append('\n');
        file.write(line);
    }
}
