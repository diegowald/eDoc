#ifndef TCPPROXYFILEID_H
#define TCPPROXYFILEID_H

#include <QObject>
#include "../eDoc-API/IDocID.h"
#include <QUuid>

class TCPProxyFileID : public QObject, public IDocID
{
    Q_OBJECT
public:
    explicit TCPProxyFileID(QObject *parent = 0);
    TCPProxyFileID(const QString &stringID, QObject *parent = 0);
    virtual ~TCPProxyFileID();
    virtual QString asString() const;
signals:
    
public slots:

private:
    QUuid m_ID;
    
};

#endif // TCPPROXYFILEID_H

