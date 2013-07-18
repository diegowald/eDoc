#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "../eDoc-API/IDocEngine.h"
#include "../eDoc-API/IDocument.h"
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

IDocID *MainWindow::addDocument(const QByteArray &blob)
{
    IDocEngine *e = f.docEngine();
    return  e->addDocument(blob);
}

void MainWindow::on_pushButton_pressed()
{
    IDocEngine *e = f.docEngine();
    QString text = ui->textEdit->toHtml();
    QByteArray x = text.toUtf8();
    IDocID *id = addDocument(x);
    IDocBase *doc = e->getDocument(id);
    QByteArray y;
    if (!doc->isComplex())
    {
        IDocument *d = (IDocument*)(doc);
        y = d->blob();
    }
    QString textRetrieved(y);

    QLOG_TRACE() << "Texto antes: " << text;
    QLOG_TRACE() << "Texto despues: " << textRetrieved;

    Q_ASSERT(text == textRetrieved);
}

void MainWindow::on_pushButton_2_pressed()
{
    IDatabase* db = f.databaseEngine();
    IDocEngine *e = f.docEngine();
    IRecord *rec = db->createEmptyRecord();
    rec->value("campo1")->setValue("Hola Mundo");
    rec->value("campo2")->setValue("Valor del campo2");

    QString text = ui->textEdit->toHtml();
    QByteArray x = text.toUtf8();

    IDocID *docId = addDocument(x);
    QVariant doc;
    IDocument * iDoc = (IDocument*)e->getDocument(docId);
    doc.setValue(iDoc);
    rec->value("archivo")->setValue(doc);

    QLOG_TRACE() << rec->value("campo1")->asVariant();
    QLOG_TRACE() << rec->value("campo2")->asVariant();

    QLOG_TRACE() << rec->value("campo1")->content();
    QLOG_TRACE() << rec->value("campo1")->content();
    QLOG_TRACE() << rec->value("archivo")->content();

    //rec->value("campo1")->setNull();
    QLOG_TRACE() << rec->value("campo1")->asVariant();

    IRecordID *id = db->addRecord(rec);
    QLOG_TRACE() << id->asString();
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


