#include "task.h"

Task::Task(const QString &appPath, QObject *parent) :
    QObject(parent)
{
    m_ApplicationPath = appPath;
}

void Task::run()
{
    f.initialize(m_ApplicationPath, "./console.conf.xml", &logger);

    emit finished();
}
