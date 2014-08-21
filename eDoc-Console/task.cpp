#include "task.h"
#include <QsLog.h>
#include <unistd.h>
#include <QCoreApplication>

Task::Task(const QString &appPath, QObject *parent) :
    QObject(parent)
{
    QLOG_TRACE() << "Task::Task(const QString &appPath, QObject *parent)";
    m_ApplicationPath = appPath;
    f.initialize(m_ApplicationPath, "./console.conf.xml", &logger);
    _server = f.serverEngine();

    connect(&logger, SIGNAL(LogDebug(QString)), this, SLOT(on_LogDebug(QString)));
    connect(&logger, SIGNAL(LogError(QString)), this, SLOT(on_LogError(QString)));
    connect(&logger, SIGNAL(LogFatal(QString)), this, SLOT(on_LogFatal(QString)));
    connect(&logger, SIGNAL(LogInfo(QString)), this, SLOT(on_LogInfo(QString)));
    connect(&logger, SIGNAL(LogTrace(QString)), this, SLOT(on_LogTrace(QString)));
    connect(&logger, SIGNAL(LogWarning(QString)), this, SLOT(on_LogWarning(QString)));
}

Task::~Task()
{
    delete _server;
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
