#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "../eDoc-API/IDocEngine.h"
#include "../eDoc-API/IDocument.h"
#include <QApplication>
#include <QsLog.h>
#include "../eDoc-ClientComponents/recordeditor.h"
#include "dlgadddocument.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include "../eDoc-API/IFieldDefinition.h"

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

    fillFieldsCombo();
    fillOperatorsCombo();
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


    RecordEditor *r = new RecordEditor(this);
    r->setRecord(rec);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(r);
    ui->frame->setLayout(layout);


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



void MainWindow::on_actionAdd_Document_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Select file",
                                                    ".");
    if (filename.length() >= 0)
    {
        IDatabase* db = f.databaseEngine();
        IDocEngine *e = f.docEngine();
        IRecord *rec = db->createEmptyRecord();

        DlgAddDocument dlg(this);
        dlg.setData(filename, rec);
        if (dlg.exec() == QDialog::Accepted)
        {
            dlg.applyData(rec);
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QByteArray blob = file.readAll();

            IDocID *docId = addDocument(blob);
            QVariant doc;
            IDocument * iDoc = (IDocument*)e->getDocument(docId);
            doc.setValue(iDoc);
            rec->value("archivo")->setValue(doc);

            db->addRecord(rec);
        }
    }
}


void MainWindow::fillFieldsCombo()
{
    ui->cboField->clear();

    foreach (IFieldDefinition* fDef, f.databaseEngine()->fields())
    {
        if (fDef->isVisible())
            ui->cboField->addItem(fDef->name());
    }
}

void MainWindow::fillOperatorsCombo()
{
    ui->cboOperator->clear();
    ui->cboOperator->addItem("=", EQUALS_TO);
    ui->cboOperator->addItem("<>", DISTINT_TO);
    ui->cboOperator->addItem("<", LESS_THAN);
    ui->cboOperator->addItem("<=", LESS_THAN_OR_EQUALS_TO);
    ui->cboOperator->addItem(">", GREATER_THAN);
    ui->cboOperator->addItem(">=", GREATER_THAN_OR_EQUALS_TO);
    ui->cboOperator->addItem("between", BETWEEN);
    ui->cboOperator->addItem("contains", CONTAINS);
    ui->cboOperator->addItem("starts with", STARTS_WITH);
    ui->cboOperator->addItem("ends with", ENDS_WITH);
    ui->cboOperator->addItem("is Null", IS_NULL);
    ui->cboOperator->addItem("is not Null", IS_NOT_NULL);
}

void MainWindow::on_cboOperator_currentIndexChanged(int index)
{
    VALIDQUERY op = (VALIDQUERY)ui->cboOperator->itemData(index).toInt();
    bool show2ndParameter = (op == BETWEEN);
    ui->lblAnd->setVisible(show2ndParameter);
    ui->searchValue2->setVisible(show2ndParameter);
}

void MainWindow::on_btnSearch_released()
{
    IParameter *param = f.databaseEngine()->createEmptyParameter();
    IFieldDefinition* fDef = f.databaseEngine()->field(ui->cboField->currentText());

    IValue* value1 = fDef->createEmptyValue();
    value1->setValue(QVariant(ui->searchValue1->text()));

    VALIDQUERY queryType = (VALIDQUERY) ui->cboOperator->itemData(ui->cboOperator->currentIndex()).toInt();

    IValue *value2 = NULL;
    if (queryType == BETWEEN)
    {
        value2 = fDef->createEmptyValue();
        value2->setValue(QVariant(ui->searchValue2->text()));
    }

    param->setField(fDef);
    param->setQueryType(queryType);
    param->addValue(value1);
    if (queryType == BETWEEN)
        param->addValue(value2);


    ui->searchResult->clear();

    QList<IParameter *> query;
    query.append(param);
    QList<IRecordID*> result = f.databaseEngine()->search(query);

    foreach (IRecordID *id, result)
    {
        IRecord *rec = f.databaseEngine()->getRecord(id);
        ui->searchResult->addItem(rec->value("campo1")->asVariant().toString());
    }
}
