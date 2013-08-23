#include "task.h"
#include <QsLog.h>

Task::Task(const QString &appPath, QObject *parent) :
    QObject(parent)
{
    QLOG_TRACE() << "Task::Task(const QString &appPath, QObject *parent)";
    m_ApplicationPath = appPath;
}

void Task::run()
{
    QLOG_TRACE() << "void Task::run()";
    f.initialize(m_ApplicationPath, "./console.conf.xml", &logger);

    emit finished();
}
