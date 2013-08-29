#include "task.h"
#include <QsLog.h>
#include <unistd.h>
#include <QCoreApplication>
#include "../tcpMessages/tcpcommunicator.h"

Task::Task(const QString &appPath, QObject *parent) :
    QObject(parent)
{
    QLOG_TRACE() << "Task::Task(const QString &appPath, QObject *parent)";
    m_ApplicationPath = appPath;
    f.initialize(m_ApplicationPath, "./console.conf.xml", &logger);
    establishedCommunications.clear();
}

Task::~Task()
{
    foreach (TCPCommunicator* comm, establishedCommunications)
    {
        comm->deleteLater();
    }
}

void Task::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);

    if (!tcpServer->listen(QHostAddress::Any, 55555))
    {
        QLOG_TRACE() << "Error: " << tcpServer->errorString();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    QLOG_INFO() << QString("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort());
}

void Task::run()
{
    QLOG_TRACE() << "void Task::run()";

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

        QLOG_TRACE() << "Opening network session.";
        networkSession->open();
    } else {
        sessionOpened();
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    int i = 0;
    while (i < 100)
    {
        sleep(1);
        QLOG_TRACE() << "void Task::run()";
        QLOG_TRACE() << tcpServer->errorString();
        QLOG_TRACE() << tcpServer->hasPendingConnections();
        QLOG_TRACE() << "count = " << i;
        i++;
        QCoreApplication::processEvents();
    }

    emit finished();
}

void Task::newConnection()
{
    QLOG_TRACE() << "new Connection";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    //! [4] //! [6]
        out << (quint16)0;
        out << "hello world";
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
    //! [6] //! [7]

        establishedCommunications.push_back(new TCPCommunicator(tcpServer->nextPendingConnection(),
                                                                &f, &logger, this));


/*        clientConnection->write(block);
        clientConnection->disconnectFromHost();*/
    //! [5]
}
