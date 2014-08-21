#ifndef HEADER_H
#define HEADER_H

#include <QObject>
#include "edoctcpmessages_global.h"
#include "messagecodes.h"

class EDOCTCPMESSAGESSHARED_EXPORT
        Header : public QObject
{
    Q_OBJECT
public:
    explicit Header(QObject *parent = 0);
    virtual ~Header();

    friend QDataStream& operator<<(QDataStream& os, Header &obj);
    friend QDataStream& operator>>(QDataStream& is, Header &obj);

    void setCommand(MessageCodes::CodeNumber code);
    MessageCodes::CodeNumber command();
signals:

public slots:

private:
    int magicNumber;
    MessageCodes::CodeNumber cmd;
};

QDataStream& operator<<(QDataStream& os, Header &obj);
QDataStream& operator>>(QDataStream& is, Header &obj);

#endif // HEADER_H
