#include "messageBase.h"
#include <QIODevice>

MessageBase::MessageBase(MessageType type, QObject *parent) :
    QObject(parent), messageType(type)
{
    ds = new QDataStream(&block, QIODevice::WriteOnly);
    ds->setVersion(QDataStream::Qt_4_8);
}

MessageBase::~MessageBase()
{
    delete ds;
}

QByteArray MessageBase::asBlob()
{
    return block;
}

void MessageBase::writeHeader()
{
    (*ds) << messageType;
    (*ds) << messageSize;
}
