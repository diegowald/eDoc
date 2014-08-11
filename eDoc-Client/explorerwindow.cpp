#include "explorerwindow.h"
#include "ui_explorerwindow.h"
#include <QsLog.h>
#include "../eDoc-ClientComponents/recordeditor.h"
#include "../eDoc-API/IDocument.h"
#include "../eDoc-MetadataFramework/valuedefinitions.h"
#include <QFileDialog>
#include "dlgadddocument.h"
#include <QVariant>

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
    ui->treeStructure->setColumnCount(1);

    f.initialize(QApplication::applicationDirPath(), "./client.conf.xml", &logger);

    fillFieldsCombo();
    fillOperatorsCombo();
    fillTreeCombo();
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
        if (fDef->isVisible() && fDef->isQueryable())
            ui->cboField->addItem(fDef->name());
    }
}


void ExplorerWindow::fillTreeCombo()
{
    ui->cboTree->clear();
    foreach (QString view, f.queryEngine()->getTreeNames())
    {
        ui->cboTree->addItem(view);
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

IParameter *ExplorerWindow::createSearchParameter(const QString &fieldName, VALIDQUERY queryType, QVariant value1, QVariant value2)
{
    IParameter *param = f.databaseEngine()->createEmptyParameter();
    IFieldDefinition* fDef = f.databaseEngine()->field(fieldName);

    IValue* v1 = fDef->createEmptyValue();
    v1->setValue(value1);

    IValue *v2 = NULL;
    if (queryType == BETWEEN)
    {
        v2 = fDef->createEmptyValue();
        v2->setValue(value2);
    }

    param->setField(fDef);
    param->setQueryType(queryType);
    param->addValue(v1);
    if (queryType == BETWEEN)
        param->addValue(v2);

    return param;
}

void ExplorerWindow::on_btnAddToSearch_released()
{
    VALIDQUERY queryType = (VALIDQUERY) ui->cboOperator->itemData(ui->cboOperator->currentIndex()).toInt();

    searchFilter.append(createSearchParameter(ui->cboField->currentText(),
                                              queryType, QVariant(ui->searchValue1->text()),
                                              QVariant(ui->searchValue2->text())));
    /*
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

    searchFilter.append(param);*/

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
    doSearch(searchFilter);
}

void ExplorerWindow::doSearch(QList<IParameter *> &filter)
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
    searchFilter.clear();
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
    connect(r, SIGNAL(downloadFile(IRecord*,const IValue*)), this, SLOT(downloadFile(IRecord*,const IValue*)));
    connect(r, SIGNAL(uploadFile(IRecord*,const IValue*)), this, SLOT(uploadFile(IRecord*,const IValue*)));

    //r->setEnabledEdition(false);
    r->setRecord(rec);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(r);
    ui->frameProperties->setLayout(layout);
}

void ExplorerWindow::on_actionAdd_Document_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Select file"), ".");

    if (filenames.count() > 0)
    {
        IDatabase* db = f.databaseEngine();
        IDocEngine *e = f.docEngine();

        foreach (QString filename, filenames)
        {
            IRecord *rec = db->createEmptyRecord();

            DlgAddDocument dlg(this);
            dlg.setData(filename, rec);
            bool saveFile = dlg.exec() == QDialog::Accepted;
            if (saveFile)
            {
                dlg.applyData(rec);
                QFile file(filename);
                file.open(QIODevice::ReadOnly);
                QByteArray blob = file.readAll();

                IDocID *docId = e->addDocument(blob);
                IDocument *doc = (IDocument*)e->getDocument(docId);

                rec->value("archivo")->setValue(doc->id()->asString());

                db->addRecord(rec);
            }
        }
    }
}

void ExplorerWindow::downloadFile(IRecord* record, const IValue* value)
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), ".");
    IDocEngine *e = f.docEngine();

    IDocumentIDValue *v = (IDocumentIDValue*) record->value("archivo");
    IDocID *docId = e->IValueToIDocId(v);
    IDocBase *doc = e->getDocument(docId);
    if (!doc->isComplex())
    {
        IDocument *document = (IDocument*)doc;
        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        file.write(document->blob());
        file.close();
    }
}

void ExplorerWindow::uploadFile(IRecord* record, const IValue *value)
{

}

void ExplorerWindow::on_cboTree_currentIndexChanged(const QString &arg1)
{
    (void)arg1;
    ui->treeStructure->clearSelection();
    ui->treeStructure->clear();
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeStructure, QStringList(QString("/")));
    ui->treeStructure->addTopLevelItem(item);
    fillSubTree(item);
    updateTreeFilter(item);
    doSearch(treefilter);
}

void ExplorerWindow::on_treeStructure_itemSelectionChanged()
{
    if (ui->treeStructure->selectedItems().count() > 0)
    {
        fillSubTree(ui->treeStructure->selectedItems().at(0));
        updateTreeFilter(ui->treeStructure->selectedItems().at(0));
        doSearch(treefilter);
        ui->treeStructure->selectedItems().at(0)->setExpanded(true);
    }
}

void ExplorerWindow::updateTreeFilter(QTreeWidgetItem *node)
{
    treefilter.clear();
    QList<QPair<QString, QString> > filter = getTreeFilter(node);
    QPair<QString, QString> filterParam;
    foreach (filterParam, filter)
    {
        treefilter.append(createSearchParameter(filterParam.first, EQUALS_TO, QVariant(filterParam.second), QVariant("")));
    }
    QStringList fieldsForTree = f.queryEngine()->getFieldsForTree(ui->cboTree->currentText());
    if (fieldsForTree.count() > filter.count())
    {
        QString field = f.queryEngine()->getFieldsForTree(ui->cboTree->currentText()).at(filter.count());
        treefilter.append(createSearchParameter(field, IS_NULL, QVariant(""), QVariant("")));
    }
}

void ExplorerWindow::fillSubTree(QTreeWidgetItem *parent)
{
    if (parent)
    {
        qDeleteAll(parent->takeChildren());
    }
    QList<QPair<QString, QString> > filter = getTreeFilter(parent);

    QStringList fieldsForTree = f.queryEngine()->getFieldsForTree(ui->cboTree->currentText());
    if (fieldsForTree.count() > filter.count())
    {
        QString field = f.queryEngine()->getFieldsForTree(ui->cboTree->currentText()).at(filter.count());
        QStringList values = f.databaseEngine()->getDistinctColumnValues(filter, field);
        foreach (QString value, values)
        {
            QTreeWidgetItem *item = NULL;
            if (parent)
            {
                item = new QTreeWidgetItem(parent, QStringList(value));
                parent->addChild(item);
            }
            else
            {
                item = new QTreeWidgetItem(ui->treeStructure, QStringList(value));
                ui->treeStructure->addTopLevelItem(item);
            }
            item->setData(0, Qt::UserRole, field);
        }
    }
}

QList<QPair<QString, QString> > ExplorerWindow::getTreeFilter(QTreeWidgetItem *parent)
{
    QTreeWidgetItem *currentNode = parent;
    QList<QPair<QString, QString> > list;
    while (currentNode && currentNode->parent())
    {
        QPair<QString, QString> value;
        value.first = currentNode->data(0, Qt::UserRole).toString();
        value.second = currentNode->text(0);
        list.push_front(value);
        currentNode = currentNode->parent();
    }
    return list;
}

void ExplorerWindow::on_actionAdd_1000_Documents_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Select file"), ".");

    if (filenames.count() > 0)
    {
        IDatabase* db = f.databaseEngine();
        IDocEngine *e = f.docEngine();

        QString filename = filenames.at(0);

        for (int i = 0; i < 100000; ++i)
        {
            IRecord *rec = db->createEmptyRecord();

            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QByteArray blob = file.readAll();

            IDocID *docId = e->addDocument(blob);
            IDocument *doc = (IDocument*)e->getDocument(docId);

            rec->value("archivo")->setValue(doc->id()->asString());

            rec->value("campo1")->setValue(QVariant(QString("Campo1 %1").arg(i % 20)));
            rec->value("campo2")->setValue(QVariant(QString("Campo2 %1").arg(i % 7)));

            db->addRecord(rec);
        }
    }
}

