#include "tcpclientplugin.h"
#include <../eDoc-Configuration/xmlelement.h>
#include <../eDoc-Configuration/xmlcollection.h>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QSettings>

#include "../tcpMessages/tcpAddDocumentRequest.h"

/*ver estos links
http://doc.qt.digia.com/solutions/4/qtservice/qtservice-example-server.html
http://qt-project.org/doc/qt-5.0/qtnetwork/fortuneclient-client-cpp.html*/


TCPClientPlugin::TCPClientPlugin(QObject *parent) :
    QObject(parent)
{
}

TCPClientPlugin::~TCPClientPlugin()
{
}

void TCPClientPlugin::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
    url = ((XMLElement*)((XMLCollection*)configuration)->get("url"))->value();
    port = ((XMLElement*)((XMLCollection*)configuration)->get("port"))->value().toInt();
    m_Logger = logger;
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readInfo()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(handleError(QAbstractSocket::SocketError)));

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        /*getFortuneButton->setEnabled(false);
        statusLabel->setText(tr("Opening network session."));*/
        networkSession->open();
    }

}

IDocID* TCPClientPlugin::addDocument(const QByteArray& blob)
{
    if (!tcpSocket->isOpen())
        tcpSocket->open(QTcpSocket::ReadWrite);

    tcpSocket->write()
}

IDocBase* TCPClientPlugin::getDocument(IDocID *id)
{
}

bool TCPClientPlugin::deleteDocument(IDocID *id)
{
}

QString TCPClientPlugin::name()
{
    return "tcpClientPlugin";
}

void TCPClientPlugin::readInfo()
{
}

void TCPClientPlugin::handleError(QAbstractSocket::SocketError socketError)
{

}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDoc-tcpClient, TCPClientPlugin)
#endif // QT_VERSION < 0x050000
