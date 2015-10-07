#include <QDebug>
#include <QString>
#include <QLocale>
#include <QFile>
#include <QTextStream>

#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <QtTreePropertyBrowser>

#include "treepropertywidget.h"

#include "treeleftsideinfofile.h"
#include "properynode.h"

TreePropertyWidget::TreePropertyWidget(const QString &_propertiesFile, QWidget *_parent)
    : QtTreePropertyBrowser(_parent),
      m_normalised(false),
      m_metaInfo(NULL),
      m_variantManager(new QtVariantPropertyManager())
{
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    setFactoryForManager(m_variantManager, variantFactory);
    setPropertiesWithoutValueMarked(true);
    setRootIsDecorated(false);

    fillMetadata(_propertiesFile);
}

QString TreePropertyWidget::averageJudgeName()
{
    return "Average";
}

void TreePropertyWidget::fillMetadata(const QString &_fileName)
{
    // remove previous nodes here!
    m_metaInfo = getTreeLedtSideInfo(_fileName);
    const QList<ProperyNode *> nodes = m_metaInfo->nodes();

    foreach(ProperyNode* node, nodes)
        addProperty(toProperty(node));
}

void TreePropertyWidget::setCurrentJudge(const QString &_name, bool _normalise)
{
    storeJudge(m_currentJudgeName);
    displayJudge(_name, _normalise);
}

QString TreePropertyWidget::currentJudge()
{
    return m_currentJudgeName;
}

QList<double> TreePropertyWidget::values()
{
    storeJudge(currentJudge());
    return values(currentJudge());
}

QList<double> TreePropertyWidget::values(const QString &_judgeName)
{
    QList<double> ans;
    if(!m_sourceJudges.contains(_judgeName))
        return ans;

    Judge jud = m_sourceJudges[_judgeName];
    QStringList orderedKeys = m_metaInfo->planeNodes();
    foreach (QString key, orderedKeys)
    {
        foreach(QtProperty* prop, jud.keys())
        {
            if(prop->propertyName() == key)
                ans << toDouble(prop->valueText());
        }
    }
    return ans;
}

void TreePropertyWidget::valuesToFile(const QString &_judgeName, const QString &_file)
{
    QList<double> vals = values(_judgeName);
    QFile file(_file);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    foreach (double val, vals)
        out << val;
}

void TreePropertyWidget::setValues(const QString &_judgeName, const QList<double> &_values)
{
    Judge jud = emptyJudge();

//    Q_ASSERT(jud.size() != _values.size());

    QStringList orderedKeys = m_metaInfo->planeNodes();

    int size = qMin(jud.size(), _values.size());
    for(int i = 0; i < size; ++i)
    {
        QString key = orderedKeys.at(i);
        QMapIterator<QtProperty*, QVariant> p(jud);
        while (p.hasNext())
        {
            p.next();
            if(p.key()->propertyName() == key)
            {
                jud[p.key()] = _values.at(i);
                break;
            }
        }
    }
    m_sourceJudges[_judgeName] = jud;
}

void TreePropertyWidget::readValues(const QString &_judgeName, const QString &_file)
{
    QFile file(_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QList<double> vals;
    QTextStream in(&file);
    while(!in.atEnd())
    {
        double d;
        in >> d;
        vals << d;
    }
    setValues(_judgeName, vals);
}

void TreePropertyWidget::setAverageCalculation(bool _normalise)
{
    storeJudge(m_currentJudgeName);

    Judge average;

    // Записали сумму по всем судьям
    QMapIterator<QString, Judge> j(m_sourceJudges);
    while (j.hasNext())
    {
        j.next();
        if(j.key() == averageJudgeName())
            continue;

        Judge jud = j.value();
        if(_normalise)
            jud = normalise(jud);
        QMapIterator<QtProperty*, QVariant> p(jud);
        while (p.hasNext())
        {
            p.next();
            if(!average.contains(p.key()))
                average[p.key()] = 0.0;

            average[p.key()] = toDouble(p.value()) + toDouble(average[p.key()]);
        }
    }

    // Поделили на количество:
    int count = m_sourceJudges.count();
    QMapIterator<QtProperty*, QVariant> p(average);
    while (p.hasNext())
    {
        p.next();
        double val = toDouble(p.value());
        val /= count;
        average[p.key()] = QVariant(val);
    }
    // Отобразили:
    displayJudge(average);
    m_currentJudgeName = averageJudgeName();
}

void TreePropertyWidget::normalise(bool _norm)
{
    setCurrentJudge(m_currentJudgeName, _norm);

    m_normalised = _norm;
    setEditable(!_norm);
}

TreePropertyWidget::Judge TreePropertyWidget::emptyJudge()
{
    Judge empty;
    foreach(QtProperty* prop, m_variantManager->properties())
        empty[prop] = double(0);
    return empty;
}

TreeLeftSideInfo *TreePropertyWidget::getTreeLedtSideInfo(const QString& _fileName)
{
    // Возможно здесь будет фабрика.
    TreeLeftSideInfo* info = new TreeLeftSideInfoFile();
    info->open(_fileName);
    return info;
}

/*!
 * \brief TreePropertyWidget::storeJudge
 * Saves judge from ui with the name _name
 * \param _name
 */
void TreePropertyWidget::storeJudge(const QString &_name)
{
    if(_name.isEmpty() || m_normalised || _name == averageJudgeName())
        return;

    if(!m_sourceJudges.contains(_name))
        m_sourceJudges[_name] = Judge();

    Judge storingJudge = m_sourceJudges.value(_name);

    foreach(QtProperty* prop, m_variantManager->properties())
        storingJudge[prop] = prop->valueText();

    m_sourceJudges[_name] = storingJudge;
}

void TreePropertyWidget::displayJudge(const QString &_name, bool _normalise)
{
//    if(m_currentJudgeName == _name)
//        return;

    if(_name == averageJudgeName())
    {
        setAverageCalculation(_normalise);
        return;
    }

    if(!m_sourceJudges.contains(_name))
        m_sourceJudges[_name] = Judge();

    Judge displaying = m_sourceJudges.value(_name);

    if(_normalise)
        displayJudge(normalise(displaying));
    else
        displayJudge(displaying);
    m_currentJudgeName = _name;
}

void TreePropertyWidget::displayJudge(const Judge &_judge)
{
    foreach(QtProperty* prop, m_variantManager->properties())
    {
        QVariant newValue = _judge.value(prop, QVariant(double(0)));
        double newDoubleVal = toDouble(newValue);
        m_variantManager->setValue(prop, newDoubleVal);
    }
}

QtProperty* TreePropertyWidget::toProperty(ProperyNode *_node)
{
    QtProperty *property = m_variantManager->addProperty(nodeType(_node), _node->description());

    foreach(ProperyNode* node, _node->children())
    {
        QtProperty* subProperty = toProperty(node);
        property->addSubProperty(subProperty);
    }

    return property;
}

int TreePropertyWidget::nodeType(const ProperyNode *_node) const
{
    bool hasChildren = !_node->children().isEmpty();
    if(hasChildren)
        return QtVariantPropertyManager::groupTypeId();
    else
        return QVariant::Double;
}

double TreePropertyWidget::toDouble(const QString &_str) const
{
    if(_str.isEmpty())
        return 0;

    bool isOk;
    double ans = QLocale::system().toDouble(_str, &isOk);
    if(!isOk)
        ans = _str.toDouble(&isOk);
    Q_ASSERT(isOk);

    return ans;
}

double TreePropertyWidget::toDouble(const QVariant &_var) const
{
    return toDouble(_var.toString());
}

TreePropertyWidget::Judge TreePropertyWidget::normalise(const Judge &_judge) const
{
    double summ = 0;
    QMapIterator<QtProperty*, QVariant> it(_judge);
    while (it.hasNext())
    {
        it.next();
        double val = toDouble(it.value());
        summ += val;
    }
    Judge normalised = _judge;
    QMapIterator<QtProperty*, QVariant> itNorm(normalised);
    while (itNorm.hasNext())
    {
        itNorm.next();
        double val = toDouble(itNorm.value());
        if(summ != 0)
            val /= summ;
        normalised[itNorm.key()] = QVariant(val);
    }
    return normalised;
}

void TreePropertyWidget::setEditable(bool _set)
{
    foreach(QtProperty* prop, m_variantManager->properties())
    {
        prop->setEnabled(_set);
    }
}
