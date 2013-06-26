#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-API/IDocEngine.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    EDocFactory f;
    f.initialize(QApplication::applicationDirPath(), "");
    IDocEngine *e = f.docEngine();
}

MainWindow::~MainWindow()
{
    delete ui;
}
