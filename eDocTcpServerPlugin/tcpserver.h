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

    virtual void initialize(IXMLContent *configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual void run();
    virtual void stop();
    virtual QString name() const;

private:
    QSharedPointer<IDatabase> createPersistentEngine(XMLCollection *confEngine,
                                      const QMap<QString, QString> &docpluginStock,
                                      const QMap<QString, QString> &DBplugins,
                                      const QMap<QString, QString> &DBWithHistoryPlugins,
                                      const QMap<QString, QString> &tagPlugins,
                                      const QMap<QString, QString> &serverPlugins);

    QSharedPointer<IDatabaseWithHistory> createHistoryDBPersistentEngine(XMLCollection *confEngine,
                                                          const QMap<QString, QString> &docpluginStock,
                                                          const QMap<QString, QString> &DBplugins,
                                                          const QMap<QString, QString> &DBWithHistoryPlugins,
                                                          const QMap<QString, QString> &tagPlugins,
                                                          const QMap<QString, QString> &serverPlugins);
    QSharedPointer<IDocEngine> createDocEnginePersistance(XMLCollection *confEngine,
                                           const QMap<QString, QString> &docpluginStock,
                                           const QMap<QString, QString> &DBplugins,
                                           const QMap<QString, QString> &DBWithHistoryPlugins,
                                           const QMap<QString, QString> &tagPlugins,
                                           const QMap<QString, QString> &serverPlugins);
signals:
public slots:

private slots:
    void onNewConnection();

private:
    QSharedPointer<QObjectLogging> logger;
    QString m_Name;
    QSharedPointer<IDatabase> database;
    QSharedPointer<IDatabaseWithHistory> dbh;
    QSharedPointer<IDocEngine> docEngine;
    QSharedPointer<QTcpServer> tcpServer;
    int port;
    QMap<QSharedPointer<QTcpSocket>, QSharedPointer<EDocTCPServerDatabasePlugin>> clientConnections;
};

#endif // TCPSERVER_H
