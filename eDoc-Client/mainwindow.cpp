#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-API/IDocEngine.h"
#include <QApplication>
#include <QsLog.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QLOG_TRACE() << "MainWindow::MainWindow(QWidget *parent)";

    ui->setupUi(this);
    EDocFactory f;
    f.initialize(QApplication::applicationDirPath(), "");
    IDocEngine *e = f.docEngine();
}

MainWindow::~MainWindow()
{
    QLOG_TRACE() << "MainWindow::~MainWindow()";
    delete ui;
}
