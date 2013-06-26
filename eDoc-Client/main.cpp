#include "mainwindow.h"
#include <QApplication>
#include <QsLog.h>
#include <QDir>

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


    MainWindow w;
    w.show();
    
    return a.exec();
}
