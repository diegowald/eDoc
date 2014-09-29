#ifndef JSONSERVER_H
#define JSONSERVER_H

#include <QObject>
#include "edocjsonserverplugin_global.h"
#include <QtNetwork/QTcpServer>
#include <QMap>
#include "../eDoc-API/forward.h"
#include "../eDoc-API/IServer.h"
#include "../eDoc-API/IDatabase.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "edocjsonserverplugin.h"

class JsonServer : public QObject, public IServer
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IServer/0.0" FILE "eDocTcpServerPlugin.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(IServer)

public:
    explicit JsonServer(QObject *parent = 0);
    virtual ~JsonServer();

    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);
    virtual void run();
    virtual void stop();
    virtual QString name() const;
    IServerPtr newServer();

private slots:
    void onNewConnection();
    void onClientConnectionFinished();

private:
    QObjectLoggingPtr logger;
    QString m_Name;
    IDatabasePtr database;
    IDatabaseWithHistoryPtr dbh;
    IDocEnginePtr docEngine;
    QSharedPointer<QTcpServer> tcpServer;
    ITagProcessorPtr tagProcessor;
    int port;
    QMap<QSharedPointer<QTcpSocket>, QSharedPointer<EDocJsonServerPlugin>> clientConnections;
};

#endif // JSONSERVER_H
