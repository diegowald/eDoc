#include "getblobresponse.h"

getBlobResponse::getBlobResponse(QObject *parent) :
    MessageBase(parent)
{
}

QDataStream& operator<<(QDataStream& dataStream, const getBlobResponse& msg)
{
    dataStream << (*(MessageBase*)&msg)
               << msg.blob;
    return dataStream;
}

// Important: this will throw a UserException on error
QDataStream& operator>>(QDataStream& dataStream, getBlobResponse& msg)
{
    dataStream >> (*(MessageBase*)&msg)
               >> msg.blob;
    return dataStream;
}
