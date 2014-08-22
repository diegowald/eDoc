#include "proxydocid.h"

ProxyDocID::ProxyDocID(QObject *parent) :
    QObject(parent)
{
    id = "";
}

ProxyDocID::~ProxyDocID()
{
}

QString ProxyDocID::asString() const
{
    return id;
}
