#include <QCoreApplication>
#include <QsLog.h>
#include <QDir>
#include "task.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QsLogging::Logger & logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);

    const QString logPath(QDir(a.applicationDirPath()).filePath("eDoc-Console.log"));


    // Create log destinations
    QsLogging::DestinationPtr fileDestination(
       QsLogging::DestinationFactory::MakeFileDestination(logPath) );
    QsLogging::DestinationPtr debugDestination(
       QsLogging::DestinationFactory::MakeDebugOutputDestination() );

    // set log destinations on the logger
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
    QLOG_TRACE() << "Starting application";

    Task *task = new Task(a.applicationDirPath(), &a);
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();
}
