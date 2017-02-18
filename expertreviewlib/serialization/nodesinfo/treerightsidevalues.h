#ifndef TREERIGHTSIDEVALUES_H
#define TREERIGHTSIDEVALUES_H

#include <QStringList>
#include <QVariant>

//!
//! \brief The TreeRightSideValues class абстрактный класс векторов значений проектов
//!
class TreeRightSideValues
{
    friend class ProjectsLoader;

public:
    TreeRightSideValues() {}
    virtual ~TreeRightSideValues() {}

    virtual QString leftSideId() const = 0;

    QString id() const { return m_id; }
    void setId(const QString& _id) { m_id = _id; }

    virtual QString guiName() const = 0;
    virtual void setGuiName(const QString& _guiName) = 0;

    bool isTemp() const { return m_isTemp; }
    void setTemp(bool temp = true) { m_isTemp = temp; }

    virtual QMap<QString, double> values() const = 0; // key - value
    virtual void setValues(const QMap<QString, double>& _values) = 0;

private:
    virtual void readValues(const QString& _id) = 0;
    virtual void writeValues(const QString& _id) = 0;

private:
    QString m_id;
    bool m_isTemp;
};

#endif // TREERIGHTSIDEVALUES_H
