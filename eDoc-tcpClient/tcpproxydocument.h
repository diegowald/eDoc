#ifndef TCPPROXYDOCUMENT_H
#define TCPPROXYDOCUMENT_H

#include <QObject>
#include "../eDoc-API/IDocument.h"
#include "tcpproxyfileid.h"
#include <QtNetwork>

class TCPProxyDocument : public QObject, public IDocument
{
    Q_OBJECT
public:
    explicit TCPProxyDocument(QTcpSocket *Socket, QObject *parent = 0);
    TCPProxyDocument(QTcpSocket *Socket, IDocID *id, QObject *parent = 0);
    virtual ~TCPProxyDocument();
    virtual IDocID* id();
    virtual QByteArray blob();

signals:
    
public slots:

private:
    IDocID *m_ID;
    QTcpSocket *tcpSocket;
};

#endif // TCPPROXYDOCUMENT_H
