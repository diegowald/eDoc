#ifndef TASK_H
#define TASK_H

#include <QObject>
#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/IDocID.h"
#include <QtNetwork>
#include "tcpcommunicator.h"
#include "QList"

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(const QString &appPath, QObject *parent = 0);
    virtual ~Task();

signals:
    void finished();

public slots:
    void run();
    
private:
    void sessionOpened();

private slots:
    void newConnection();

private:
    EDocFactory f;
    QString m_ApplicationPath;
    QObjectLogging logger;
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;
    QList<TCPCommunicator*> establishedCommunications;
};

#endif // TASK_H
