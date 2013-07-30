#include "mainwindow.h"
#include <QApplication>
#include <QsLog.h>
#include <QDir>
#include "explorerwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QsLogging::Logger & logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);

    const QString logPath(QDir(a.applicationDirPath()).filePath("eDoc-Client.log"));


    // Create log destinations
    QsLogging::DestinationPtr fileDestination(
       QsLogging::DestinationFactory::MakeFileDestination(logPath) );
    QsLogging::DestinationPtr debugDestination(
       QsLogging::DestinationFactory::MakeDebugOutputDestination() );

    // set log destinations on the logger
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
    QLOG_TRACE() << "Starting application";

/*    MainWindow w;
    w.show();*/
    ExplorerWindow w;
    w.show();
    
    int result = a.exec();
    QLOG_TRACE() << "Aplication ended. Result code: " << result;
    return result;
}
