#include "parseradaptor.h"
#include "parser.h"

namespace parser
{

ParserAdaptor::ParserAdaptor()
    : m_parser(nullptr)
{}

ParserAdaptor::~ParserAdaptor()
{
    delete m_parser;
}

QString ParserAdaptor::calculate(const QString &_expression)
{
    if(!m_parser)
        m_parser = new Parser();

    QByteArray tmp = _expression.toUtf8();
    const char* tmpRawStr = tmp.data();

//    const char* rawStr = m_parser->parse( toStdString().c_str());
    tmpRawStr = m_parser->parse(tmpRawStr);
    QString ans(tmpRawStr);
    return ans;
}

}
