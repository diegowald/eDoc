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

    ui->setupUi(this);
    f.initialize(QApplication::applicationDirPath(), "./client.conf.xml");

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
}
