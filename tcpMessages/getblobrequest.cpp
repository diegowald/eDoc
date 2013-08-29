#include "getblobrequest.h"

getBlobRequest::getBlobRequest(IDocID *Id, QObject *parent) :
    MessageBase(GET_BLOB_REQ, parent), id(Id)
{
}

QDataStream& operator<<(QDataStream& dataStream, const getBlobRequest& msg)
{
    dataStream << *((MessageBase*)&msg)
               << msg.id->asString();
    return dataStream;
}

// Important: this will throw a UserException on error
QDataStream& operator>>(QDataStream& dataStream, getBlobRequest& msg)
{
    QString id;
    dataStream >> *((MessageBase*) &msg)
               >> id;
    //msg.id = id;
    return dataStream;
}
