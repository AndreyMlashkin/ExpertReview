#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "projectapi.h"

#include "treerightsidevaluesfile.h"

TreeRightSideValuesFile::TreeRightSideValuesFile(const QStringList &_orderedKeys)
    : m_orderedKeys(_orderedKeys)
{}

QMap<QString, double> TreeRightSideValuesFile::values() const
{
    if(m_values.isEmpty())
        const_cast<TreeRightSideValuesFile*>(this)->fillWithEmptyVals();

    return m_values;
}

void TreeRightSideValuesFile::setValues(const QMap<QString, double>& _values)
{
    // Вставить проверку
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

    QTextStream in(&file);

    int count = 0;
    while(!in.atEnd())
    {
        QString line = in.readLine();

        if(m_orderedKeys.size() > count)
        {
            qDebug() << "too much values in file" << _id;
            continue;
        }

        QString key = m_orderedKeys[count];
        m_values[key] = toDouble(line);

        ++count;
    }
    if(m_orderedKeys.size() == count)
        qDebug() << "too much keys!, no enought values in file!";
}

void TreeRightSideValuesFile::writeValues(const QString &_id)
{
    if(_id.isEmpty())
        return;

    QFile file(_id);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    foreach (QString key, m_orderedKeys)
    {
        double val = m_values[key];
        QString line = QString::number(val) + '\n';
        out << line;
    }
}

void TreeRightSideValuesFile::fillWithEmptyVals()
{
    foreach(QString key, m_orderedKeys)
        m_values[key] = 0.0;
}
