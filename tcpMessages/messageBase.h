#ifndef MESSAGEBASE_H
#define MESSAGEBASE_H

#include <QObject>
#include "tcpmessages_global.h"
#include <QDataStream>

enum MessageType
{
    ADD_DOCUMENT
};

class TCPMESSAGESSHARED_EXPORT MessageBase : public QObject
{
    Q_OBJECT
public:
    explicit MessageBase(MessageType type, QObject *parent = 0);
    virtual ~MessageBase();

    int messageSize;
    MessageType messageType;

    virtual QByteArray asBlob();
    virtual void writeHeader();

protected:
    QByteArray block;
    QDataStream *ds;
};

#endif // MESSAGEBASE_H
