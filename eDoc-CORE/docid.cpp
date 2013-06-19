#include "docid.h"

DocID::DocID(QString id)
{
    value = id;
}

QString DocID::asString() const
{
    return value;
}

DocID *DocID::createNew()
{
    return new DocID("1234");
}
