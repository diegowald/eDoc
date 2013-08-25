#include "explorerwindow.h"
#include "ui_explorerwindow.h"
#include <QsLog.h>
#include "../eDoc-ClientComponents/recordeditor.h"
#include "../eDoc-API/IDocument.h"
#include <QFileDialog>
#include "dlgadddocument.h"

ExplorerWindow::ExplorerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExplorerWindow)
{
    QLOG_TRACE() << "ExplorerWindow::ExplorerWindow(QWidget *parent)";

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

ExplorerWindow::~ExplorerWindow()
{
    delete ui;
}

void ExplorerWindow::on_LogTrace(const QString& text)
{
    QLOG_TRACE() << text;
}

void ExplorerWindow::on_LogDebug(const QString& text)
{
    QLOG_TRACE() << text;
}


void ExplorerWindow::on_LogInfo(const QString& text)
{
    QLOG_INFO() << text;
}

void ExplorerWindow::on_LogWarning(const QString& text)
{
    QLOG_WARN() << text;
}

void ExplorerWindow::on_LogError(const QString& text)
{
    QLOG_ERROR() << text;
}

void ExplorerWindow::on_LogFatal(const QString& text)
{
    QLOG_FATAL() << text;
}

void ExplorerWindow::fillFieldsCombo()
{
    ui->cboField->clear();

    foreach (IFieldDefinition* fDef, f.databaseEngine()->fields())
    {
        if (fDef->isVisible())
            ui->cboField->addItem(fDef->name());
    }
}

void ExplorerWindow::fillOperatorsCombo()
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

void ExplorerWindow::on_cboOperator_currentIndexChanged(int index)
{
    VALIDQUERY op = (VALIDQUERY)ui->cboOperator->itemData(index).toInt();
    bool show2ndParameter = (op == BETWEEN);
    ui->lblAnd->setVisible(show2ndParameter);
    ui->searchValue2->setVisible(show2ndParameter);
}

void ExplorerWindow::on_btnAddToSearch_released()
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

    filter.append(param);
    QString displayFormat = "%1 %2 %3%4";
    QString display = displayFormat.arg(ui->cboField->currentText())
            .arg(ui->cboOperator->currentText())
            .arg(ui->searchValue1->text())
            .arg((queryType == BETWEEN ?
                      (QString(" AND %1").arg(ui->searchValue2->text())) :
                      ""));
    ui->searchFilters->addItem(display);
    on_btnSearchAgain_released();
}

void ExplorerWindow::on_btnSearchAgain_released()
{
    ui->searchResult->setRowCount(0);
    if (filter.size() > 0)
    {
        QList<IRecordID*> result = f.databaseEngine()->search(filter);

        foreach (IRecordID *id, result)
        {
            IRecord *rec = f.databaseEngine()->getRecord(id);
            int rowNum = ui->searchResult->rowCount();
            ui->searchResult->insertRow(rowNum);
            rowNum = ui->searchResult->rowCount() - 1;
            ui->searchResult->setItem(rowNum, 0, new QTableWidgetItem(rec->value("campo1")->asVariant().toString()));
            ui->searchResult->setItem(rowNum, 1, new QTableWidgetItem(rec->value("campo2")->asVariant().toString()));
            QVariant r;
            r.setValue(rec);
            ui->searchResult->item(rowNum, 0)->setData(Qt::UserRole, r);
        }
    }
}

void ExplorerWindow::on_btnClearSearch_released()
{
    filter.clear();
    ui->searchFilters->clear();
    on_btnSearchAgain_released();
}

void ExplorerWindow::on_searchResult_itemSelectionChanged()
{
    QList<QTableWidgetItem*> selection = ui->searchResult->selectedItems();
    if (selection.count() == 0)
        return;

    IRecord *rec = ui->searchResult->item(selection.at(0)->row(), 0)->data(Qt::UserRole).value<IRecord*>();

    RecordEditor *r = new RecordEditor(this);
    r->setEnabledEdition(false);
    r->setRecord(rec);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(r);
    ui->frameProperties->setLayout(layout);
}

void ExplorerWindow::on_actionAdd_Document_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "Select file",
                                                          ".");

    if (filenames.count() > 0)
    {
        foreach (QString filename, filenames)
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

                IDocID *docId = f.docEngine()->addDocument(blob);
                /*QVariant doc;
                IDocument * iDoc = (IDocument*)e->getDocument(docId);
                doc.setValue(iDoc);
                rec->value("archivo")->setValue(doc);

                db->addRecord(rec);*/
            }
        }
    }
}
