#ifndef EDOCTCPSERVERDATABASEPLUGIN_H
#define EDOCTCPSERVERDATABASEPLUGIN_H

#include "edoctcpserverplugin_global.h"
#include <QThread>
#include <QtNetwork/QTcpServer>

#include "../eDoc-API/IDatabase.h"
#include "../eDoc-API/IDocEngine.h"

#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDocTCPMessages/header.h"
#include "../eDocTCPMessages/messagecodes.h"

class EDOCTCPSERVERPLUGIN_EXPORT EDocTCPServerDatabasePlugin : public QThread
{
    Q_OBJECT
    //Q_INTERFACES(IDatabase)

public:
    EDocTCPServerDatabasePlugin(QObjectLogging *Logger, QTcpSocket *socket, IDatabase *persistance, IDocEngine* docEngine, QObject *parent = 0);
    virtual ~EDocTCPServerDatabasePlugin();

    void run();

private slots:
    void onReadyRead();
    void error(QAbstractSocket::SocketError);
    void connected();

private:
    void process(QDataStream &in);

    void prepareToSend(MessageCodes::CodeNumber code);
    void send();

private:
    QObjectLogging *logger;
    QString m_Name;
    QTcpSocket *_socket;
    IDatabase *_persistance;
    IDocEngine *_docEngine;
    int blockSize;

    QByteArray blob;

    QByteArray buildingBlob;
    QDataStream *out;

};

#endif // EDOCTCPSERVERDATABASEPLUGIN_H
