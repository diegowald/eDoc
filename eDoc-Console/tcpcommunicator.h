#ifndef TCPCOMMUNICATOR_H
#define TCPCOMMUNICATOR_H

#include <QObject>
#include <QtNetwork>

class TCPCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit TCPCommunicator(QTcpSocket *socket,
                             EDocFactory *f,
                             QObjectLogging *logger,
                             QObject *parent = 0);
    virtual ~TCPCommunicator();

private slots:
    void readyRead();

private:
    void parse();
private:
    QTcpSocket *m_Socket;
    EDocFactory *m_DocFactory;
    QObjectLogging *m_logger;
    QByteArray readBlock;
};

#endif // TCPCOMMUNICATOR_H
