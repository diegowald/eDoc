#include "header.h"
#include <QStringList>

Header::Header(QObject *parent) :
    QObject(parent), magicNumber(1234), cmd(MessageCodes::CodeNumber::ERROR)
{
}

Header::~Header()
{
}

QDataStream& operator<<(QDataStream& os, Header &obj)
{
    os << obj.magicNumber;
    os << (int) obj.cmd;
    return os;
}

QDataStream& operator>>(QDataStream& is, Header &obj)
{
    int magic;
    is >> magic;
    if (magic == obj.magicNumber)
    {
        int cm;
        is >> cm;
        obj.cmd = (MessageCodes::CodeNumber)cm;
    }
    else
    {
        // fin del mundo
    }
    return is;
}

void Header::setCommand(MessageCodes::CodeNumber code)
{
    cmd = code;
}

MessageCodes::CodeNumber Header::command()
{
    return cmd;
}
