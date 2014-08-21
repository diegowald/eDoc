#ifndef TASK_H
#define TASK_H

#include <QObject>
#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/IServer.h"

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

private slots:
    void on_LogTrace(const QString& text);
    void on_LogDebug(const QString& text);
    void on_LogInfo(const QString& text);
    void on_LogWarning(const QString& text);
    void on_LogError(const QString& text);
    void on_LogFatal(const QString& text);

private:
    EDocFactory f;
    QString m_ApplicationPath;
    QObjectLogging logger;
    IServer *_server;
};

#endif // TASK_H
