#include "proxyrecordid.h"

ProxyRecordID::ProxyRecordID(QObject *parent) :
    QObject(parent)
{
    _id = "";
}

ProxyRecordID::~ProxyRecordID()
{
}

QString ProxyRecordID::asString() const
{
    return _id;
}

IMetadata *ProxyRecordID::metadata()
{
}

void ProxyRecordID::setValue(const QString &value)
{
    _id = value;
}
