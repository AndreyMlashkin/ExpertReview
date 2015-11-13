#ifndef PARSERADAPTOR_H
#define PARSERADAPTOR_H

#include <QString>
#include "export.h"

namespace parser
{

class Parser;
class EXPRESSIONCALCULATOR_EXPORT ParserAdaptor
{
public:
    ParserAdaptor();
    ~ParserAdaptor();
    QString calculate(const QString& _expression);

private:
    Parser* m_parser;
};

}

#endif // PARSERADAPTOR_H
