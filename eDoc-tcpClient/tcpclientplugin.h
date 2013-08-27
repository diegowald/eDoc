#ifndef HTTPCLIENTPLUGIN_H
#define HTTPCLIENTPLUGIN_H

#include <IDocEngine.h>
#include <QMap>
#include <../eDoc-Configuration/xmlcollection.h>
#include <QTcpSocket>
#include <QNetworkSession>
#include <QDataStream>
#include <../tcpMessages/messageBase.h>

class TCPClientPlugin : public QObject, public IDocEngine
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDocEngine/0.0" FILE "eDoc-HttpClient.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDocEngine)
public:
    TCPClientPlugin(QObject *parent = 0);

    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);
    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocBase* getDocument(IDocID *id);
    virtual bool deleteDocument(IDocID *id);
    virtual QString name();
    virtual ~TCPClientPlugin();

private:
    void sendData(MessageBase *msg);


private slots:
    void readInfo();
    void handleError(QAbstractSocket::SocketError socketError);

private:
    QString url;
    int port;
    QObjectLogging *m_Logger;

    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;

};

#endif // HTTPCLIENTPLUGIN_H

