#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "edoctcpserverplugin_global.h"
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QMap>
#include "../eDoc-API/IServer.h"
#include "../eDoc-API/IDatabase.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "edoctcpserverdatabaseplugin.h"

class EDOCTCPSERVERPLUGIN_EXPORT TcpServer : public QObject, public IServer
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IServer/0.0" FILE "eDocTcpServerPlugin.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(IServer)

public:
    explicit TcpServer(QObject *parent = 0);
    virtual ~TcpServer();

    virtual void initialize(IXMLContentPtr configuration,
                            QObjectLoggingPtr logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual void run();
    virtual void stop();
    virtual QString name() const;

private:
    IDatabasePtr createPersistentEngine(XMLCollectionPtr confEngine,
                                        const QMap<QString, QString> &docpluginStock,
                                        const QMap<QString, QString> &DBplugins,
                                        const QMap<QString, QString> &DBWithHistoryPlugins,
                                        const QMap<QString, QString> &tagPlugins,
                                        const QMap<QString, QString> &serverPlugins);

    IDatabaseWithHistoryPtr createHistoryDBPersistentEngine(XMLCollectionPtr confEngine,
                                                            const QMap<QString, QString> &docpluginStock,
                                                            const QMap<QString, QString> &DBplugins,
                                                            const QMap<QString, QString> &DBWithHistoryPlugins,
                                                            const QMap<QString, QString> &tagPlugins,
                                                            const QMap<QString, QString> &serverPlugins);

    IDocEnginePtr createDocEnginePersistance(XMLCollectionPtr confEngine,
                                             const QMap<QString, QString> &docpluginStock,
                                             const QMap<QString, QString> &DBplugins,
                                             const QMap<QString, QString> &DBWithHistoryPlugins,
                                             const QMap<QString, QString> &tagPlugins,
                                             const QMap<QString, QString> &serverPlugins);

    ITagProcessorPtr createTagProcessor(XMLCollectionPtr confEngine,
                                        const QMap<QString, QString> &docpluginStock,
                                        const QMap<QString, QString> &DBplugins,
                                        const QMap<QString, QString> &DBWithHistoryPlugins,
                                        const QMap<QString, QString> &tagPlugins,
                                        const QMap<QString, QString> &serverPlugins);

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
    QMap<QSharedPointer<QTcpSocket>, QSharedPointer<EDocTCPServerDatabasePlugin>> clientConnections;
};

#endif // TCPSERVER_H
