#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "../eDoc-API/IDocEngine.h"
#include <QApplication>
#include <QsLog.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QLOG_TRACE() << "MainWindow::MainWindow(QWidget *parent)";

    connect(&logger, SIGNAL(LogDebug(QString)), this, SLOT(on_LogDebug(QString)));
    connect(&logger, SIGNAL(LogError(QString)), this, SLOT(on_LogError(QString)));
    connect(&logger, SIGNAL(LogFatal(QString)), this, SLOT(on_LogFatal(QString)));
    connect(&logger, SIGNAL(LogInfo(QString)), this, SLOT(on_LogInfo(QString)));
    connect(&logger, SIGNAL(LogTrace(QString)), this, SLOT(on_LogTrace(QString)));
    connect(&logger, SIGNAL(LogWarning(QString)), this, SLOT(on_LogWarning(QString)));

    ui->setupUi(this);
    f.initialize(QApplication::applicationDirPath(), "./client.conf.xml", &logger);

}

MainWindow::~MainWindow()
{
    QLOG_TRACE() << "MainWindow::~MainWindow()";
    delete ui;
}

void MainWindow::on_pushButton_pressed()
{
    IDocEngine *e = f.docEngine();
    QString text = ui->textEdit->toHtml();
    QByteArray x = text.toUtf8();
    IDocID *id = e->addDocument(x);
    IDocument *doc = e->getDocument(id);
    QByteArray y = doc->blob();
    QString textRetrieved(y);

    QLOG_TRACE() << "Texto antes: " << text;
    QLOG_TRACE() << "Texto despues: " << textRetrieved;

    Q_ASSERT(text == textRetrieved);
}

void MainWindow::on_LogTrace(const QString& text)
{
    QLOG_TRACE() << text;
}

void MainWindow::on_LogDebug(const QString& text)
{
    QLOG_DEBUG() << text;
}

void MainWindow::on_LogInfo(const QString& text)
{
    QLOG_INFO() << text;
}

void MainWindow::on_LogWarning(const QString& text)
{
    QLOG_WARN() << text;
}

void MainWindow::on_LogError(const QString& text)
{
    QLOG_ERROR() << text;
}

void MainWindow::on_LogFatal(const QString& text)
{
    QLOG_FATAL() << text;
}
