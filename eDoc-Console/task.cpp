#include "task.h"
#include <QsLog.h>
#include <unistd.h>
#include <QCoreApplication>

Task::Task(const QString &appPath, QObject *parent) :
    QObject(parent)
{
    QLOG_TRACE() << "Task::Task(const QString &appPath, QObject *parent)";
    m_ApplicationPath = appPath;
    logger = QSharedPointer<QObjectLogging>(new QObjectLogging());

    f.initialize(m_ApplicationPath, "./consolejson.conf.xml", logger);
    _server = f.serverEngine();

    connect(logger.data(), SIGNAL(LogDebug(QString)), this, SLOT(on_LogDebug(QString)));
    connect(logger.data(), SIGNAL(LogError(QString)), this, SLOT(on_LogError(QString)));
    connect(logger.data(), SIGNAL(LogFatal(QString)), this, SLOT(on_LogFatal(QString)));
    connect(logger.data(), SIGNAL(LogInfo(QString)), this, SLOT(on_LogInfo(QString)));
    connect(logger.data(), SIGNAL(LogTrace(QString)), this, SLOT(on_LogTrace(QString)));
    connect(logger.data(), SIGNAL(LogWarning(QString)), this, SLOT(on_LogWarning(QString)));
}

Task::~Task()
{
}

void Task::run()
{
    QLOG_TRACE() << "void Task::run()";

    forever
    {
        _server->run();
        sleep(1);
    }

    emit finished();
}

void Task::on_LogTrace(const QString& text)
{
    QLOG_TRACE() << text;
}

void Task::on_LogDebug(const QString& text)
{
    QLOG_TRACE() << text;
}


void Task::on_LogInfo(const QString& text)
{
    QLOG_INFO() << text;
}

void Task::on_LogWarning(const QString& text)
{
    QLOG_WARN() << text;
}

void Task::on_LogError(const QString& text)
{
    QLOG_ERROR() << text;
}

void Task::on_LogFatal(const QString& text)
{
    QLOG_FATAL() << text;
}
