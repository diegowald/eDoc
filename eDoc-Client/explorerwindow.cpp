#include "explorerwindow.h"
#include "ui_explorerwindow.h"
#include <QsLog.h>

#include "../eDoc-API/IDocument.h"
#include "../eDoc-MetadataFramework/valuedefinitions.h"
#include <QFileDialog>
#include "dlgadddocument.h"
#include <QVariant>
#include "setdatetimetouse.h"

ExplorerWindow::ExplorerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExplorerWindow)
{
    QLOG_TRACE() << "ExplorerWindow::ExplorerWindow(QWidget *parent)";

    logger = QObjectLoggingPtr(new QObjectLogging());
    connect(logger.data(), SIGNAL(LogDebug(QString)), this, SLOT(on_LogDebug(QString)));
    connect(logger.data(), SIGNAL(LogError(QString)), this, SLOT(on_LogError(QString)));
    connect(logger.data(), SIGNAL(LogFatal(QString)), this, SLOT(on_LogFatal(QString)));
    connect(logger.data(), SIGNAL(LogInfo(QString)), this, SLOT(on_LogInfo(QString)));
    connect(logger.data(), SIGNAL(LogTrace(QString)), this, SLOT(on_LogTrace(QString)));
    connect(logger.data(), SIGNAL(LogWarning(QString)), this, SLOT(on_LogWarning(QString)));

    ui->setupUi(this);
    ui->treeStructure->setColumnCount(1);
    ui->searchResult->setRowCount(0);

    //openDatabase("./samantha.client.conf.xml");
    openDatabase("./indexer.conf.xml");
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
    if (f.databaseEngine() != NULL)
    {
        foreach (IFieldDefinitionPtr fDef, f.databaseEngine()->fields())
        {
            if (fDef->isVisible() && fDef->isQueryable())
                ui->cboField->addItem(fDef->name());
        }
    }
}

void ExplorerWindow::fillSearchResultColumns()
{
    ui->searchResult->clear();
    ui->searchResult->setRowCount(0);
    columns.clear();
    if (f.databaseEngine() != NULL)
    {
        foreach (IFieldDefinitionPtr fDef, f.databaseEngine()->fields())
        {
            if (fDef->isVisible())
            {
                columns.append(fDef->name());
            }
        }
        ui->searchResult->setColumnCount(columns.count());
        ui->searchResult->setHorizontalHeaderLabels(columns);
    }
}

void ExplorerWindow::fillTreeCombo()
{
    ui->cboTree->clear();

    if (!f.queryEngine().isNull())
    {
        foreach (QString view, f.queryEngine()->getTreeNames())
        {
            ui->cboTree->addItem(view);
        }
    }
}

void ExplorerWindow::fillOperatorsCombo()
{
    ui->cboOperator->clear();
    ui->cboOperator->addItem("=", (int) VALIDQUERY::EQUALS_TO);
    ui->cboOperator->addItem("<>", (int) VALIDQUERY::DISTINT_TO);
    ui->cboOperator->addItem("<", (int) VALIDQUERY::LESS_THAN);
    ui->cboOperator->addItem("<=", (int) VALIDQUERY::LESS_THAN_OR_EQUALS_TO);
    ui->cboOperator->addItem(">", (int) VALIDQUERY::GREATER_THAN);
    ui->cboOperator->addItem(">=", (int) VALIDQUERY::GREATER_THAN_OR_EQUALS_TO);
    ui->cboOperator->addItem("between", (int) VALIDQUERY::BETWEEN);
    ui->cboOperator->addItem("contains", (int) VALIDQUERY::CONTAINS);
    ui->cboOperator->addItem("starts with", (int) VALIDQUERY::STARTS_WITH);
    ui->cboOperator->addItem("ends with", (int) VALIDQUERY::ENDS_WITH);
    ui->cboOperator->addItem("is Null", (int) VALIDQUERY::IS_NULL);
    ui->cboOperator->addItem("is not Null", (int) VALIDQUERY::IS_NOT_NULL);
}

void ExplorerWindow::on_cboOperator_currentIndexChanged(int index)
{
    VALIDQUERY op = (VALIDQUERY)ui->cboOperator->itemData(index).toInt();
    bool show2ndParameter = (op == VALIDQUERY::BETWEEN);
    ui->lblAnd->setVisible(show2ndParameter);
    ui->searchValue2->setVisible(show2ndParameter);
}

IParameterPtr ExplorerWindow::createSearchParameter(const QString &fieldName, VALIDQUERY queryType, QVariant value1, QVariant value2)
{
    IParameterPtr param = f.databaseEngine()->createEmptyParameter();
    IFieldDefinitionPtr fDef = f.databaseEngine()->field(fieldName);

    IValuePtr v1 = fDef->createEmptyValue();
    v1->setValue(value1);

    IValuePtr v2;
    if (queryType == VALIDQUERY::BETWEEN)
    {
        v2 = fDef->createEmptyValue();
        v2->setValue(value2);
    }

    param->setField(fDef);
    param->setQueryType(queryType);
    param->addValue(v1);
    if (queryType == VALIDQUERY::BETWEEN)
        param->addValue(v2);

    return param;
}

void ExplorerWindow::on_btnAddToSearch_released()
{
    VALIDQUERY queryType = (VALIDQUERY) ui->cboOperator->itemData(ui->cboOperator->currentIndex()).toInt();

    searchFilter.append(createSearchParameter(ui->cboField->currentText(),
                                              queryType, QVariant(ui->searchValue1->text()),
                                              QVariant(ui->searchValue2->text())));
    QString displayFormat = "%1 %2 %3%4";
    QString display = displayFormat.arg(ui->cboField->currentText())
            .arg(ui->cboOperator->currentText())
            .arg(ui->searchValue1->text())
            .arg((queryType == VALIDQUERY::BETWEEN ?
                      (QString(" AND %1").arg(ui->searchValue2->text())) :
                      ""));
    ui->searchFilters->addItem(display);
    on_btnSearchAgain_released();
}



void ExplorerWindow::on_btnSearchAgain_released()
{
    doSearch(searchFilter);
}

void ExplorerWindow::doSearch(QList<IParameterPtr> &filter)
{
    if (columns.count() > 0)
    {
        ui->searchResult->setRowCount(0);
        if (filter.size() > 0)
        {
            QList<IRecordIDPtr> result = useCurrentTime ?
                        f.databaseEngine()->search(filter) :
                        f.databaseEngine()->searchByDate(filter, dateTimeToUse);

            QStringList ids;
            foreach (IRecordIDPtr id, result)
            {
                ids.append(id->asString());
            }
            QList<IRecordPtr> records = useCurrentTime ?
                        f.databaseEngine()->getRecords(ids) :
                        f.databaseEngine()->getRecordsByDate(ids, dateTimeToUse);

            foreach (IRecordPtr rec, records)
            {
                int rowNum = ui->searchResult->rowCount();
                ui->searchResult->insertRow(rowNum);
                rowNum = ui->searchResult->rowCount() - 1;
                for (int i = 0; i < columns.count(); ++i)
                {
                    ui->searchResult->setItem(rowNum, i, new QTableWidgetItem(rec->value(columns.at(i))->asVariant().toString()));
                }
                QVariant r;
                r.setValue(rec);
                ui->searchResult->item(rowNum, 0)->setData(Qt::UserRole, r);
            }
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

    IRecordPtr rec = ui->searchResult->item(selection.at(0)->row(), 0)->data(Qt::UserRole).value<IRecordPtr>();

    if (!recordEditor)
    {
        recordEditor = new RecordEditor(this);
        connect(recordEditor, SIGNAL(downloadFile(IRecordPtr,const IValuePtr)), this, SLOT(downloadFile(IRecordPtr,const IValuePtr)));
        connect(recordEditor, SIGNAL(uploadFile(IRecordPtr,const IValuePtr,QString &)), this, SLOT(uploadFile(IRecordPtr,const IValuePtr,QString &)));
        connect(recordEditor, SIGNAL(save(IRecordPtr)), this, SLOT(on_save(IRecordPtr)));
        connect(recordEditor, SIGNAL(cancelEdition(IRecordPtr)), this, SLOT(on_cancelEdition(IRecordPtr)));
        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(recordEditor);
        ui->frameProperties->setLayout(layout);
    }

    //r->setEnabledEdition(false);
    recordEditor->setRecord(rec, false);
}

void ExplorerWindow::on_actionAdd_Document_triggered()
{
    /*QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Select file"), ".");

    if (filenames.count() > 0)
    {
        IDatabaseWithHistoryPtr db = f.databaseEngine();
        IDocEnginePtr e = f.docEngine();

        IRecordPtr rec;
        foreach (QString filename, filenames)
        {
            rec = f.createEmptyRecord();

            DlgAddDocument dlg(this);
            dlg.setData(filename, rec);

            bool saveFile = (dlg.exec() == QDialog::Accepted);
            if (saveFile)
            {
                dlg.applyData(rec);
                f.addDocument(filename, rec);
            }
        }
    }*/


    IDatabaseWithHistoryPtr db = f.databaseEngine();

    IRecordPtr rec = f.createEmptyRecord();

    if (!recordEditor)
    {
        recordEditor = new RecordEditor(this);
        connect(recordEditor, SIGNAL(downloadFile(IRecordPtr,const IValuePtr)), this, SLOT(downloadFile(IRecordPtr,const IValuePtr)));
        connect(recordEditor, SIGNAL(uploadFile(IRecordPtr,const IValuePtr,QString &)), this, SLOT(uploadFile(IRecordPtr,const IValuePtr,QString &)));
        connect(recordEditor, SIGNAL(save(IRecordPtr,bool)), this, SLOT(on_save(IRecordPtr,bool)));
        connect(recordEditor, SIGNAL(cancelEdition(IRecordPtr)), this, SLOT(on_cancelEdition(IRecordPtr)));
        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(recordEditor);
        ui->frameProperties->setLayout(layout);
    }

    //r->setEnabledEdition(false);
    recordEditor->setRecord(rec, true);
}

void ExplorerWindow::downloadFile(IRecordPtr record, const IValuePtr value)
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), ".");
    if (filename.length() > 0)
    {
        QSharedPointer<IDocumentValue> v = value.dynamicCast<IDocumentValue>();
        QVariant v2 = v->content();
        IDocumentPtr doc = qvariant_cast<IDocumentPtr>(v2);
        if (!doc->isComplex())
        {
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            file.write(doc->blob());
            file.close();
        }
    }
}

void ExplorerWindow::uploadFile(IRecordPtr record, const IValuePtr value, QString &fileLocation)
{
    fileLocation = QFileDialog::getOpenFileName(this, tr("upload file"), ".");
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
        treefilter.append(createSearchParameter(filterParam.first, VALIDQUERY::EQUALS_TO, QVariant(filterParam.second), QVariant("")));
    }
    QStringList fieldsForTree = f.queryEngine()->getFieldsForTree(ui->cboTree->currentText());
    if (fieldsForTree.count() > filter.count())
    {
        QString field = f.queryEngine()->getFieldsForTree(ui->cboTree->currentText()).at(filter.count());
        treefilter.append(createSearchParameter(field, VALIDQUERY::IS_NULL, QVariant(""), QVariant("")));
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
        QStringList values = useCurrentTime ?
                    f.databaseEngine()->getDistinctColumnValues(filter, field) :
                    f.databaseEngine()->getDistinctColumnValuesByDate(filter, field, dateTimeToUse);

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
        IDatabaseWithHistoryPtr db = f.databaseEngine();
        IDocEnginePtr e = f.docEngine();

        QString filename = filenames.at(0);

        for (int i = 0; i < 1000; ++i)
        {
            QSharedPointer<IRecord> rec = f.createEmptyRecord();

            rec->value("campo1")->setValue(QVariant(QString("Campo1 %1").arg(i % 20)));
            rec->value("campo2")->setValue(QVariant(QString("Campo2 %1").arg(i % 7)));
            rec->value("keywords")->setValue(QVariant(QString("esta es una prueba de keywords")));

            f.addDocument(filename, rec);
        }
    }
}


void ExplorerWindow::on_btnBrowse_pressed()
{
    QString keywords = ui->txtKeyowrds->toPlainText();

    QStringList keywordsList = keywords.split(' ', QString::SkipEmptyParts);

    ITagProcessorPtr tagger = f.tagEngine();

    QSet<QString> result = tagger->findByTags(keywordsList);

    ui->searchResult->setRowCount(0);

    foreach (QString id, result)
    {
        QSharedPointer<IRecord> rec = useCurrentTime ?
                    f.databaseEngine()->getRecord(id) :
                    f.databaseEngine()->getRecordByDate(id, dateTimeToUse);
        if (!rec.isNull())
        {
            int rowNum = ui->searchResult->rowCount();
            ui->searchResult->insertRow(rowNum);
            rowNum = ui->searchResult->rowCount() - 1;
            for (int i = 0; i < columns.count(); ++i)
            {
                ui->searchResult->setItem(rowNum, i, new QTableWidgetItem(rec->value(columns.at(i))->asVariant().toString()));
            }
            QVariant r;
            r.setValue(rec);
            ui->searchResult->item(rowNum, 0)->setData(Qt::UserRole, r);
        }
    }
}


void ExplorerWindow::on_actionSet_Query_Date_and_Time_triggered()
{
    setDateTimeToUse dlg(this);
    dlg.setData(dateTimeToUse, useCurrentTime);
    if (dlg.exec() == QDialog::Accepted)
    {
        useCurrentTime = dlg.isUsingCurrent();
        dateTimeToUse = dlg.getDateAndTime();
    }
}

void ExplorerWindow::on_actionImport_Folder_triggered()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Import folder"), ".");
    QStringList folderParts = folder.split("/", QString::SplitBehavior::KeepEmptyParts);
}

void ExplorerWindow::on_actionOpen_database_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open database"), ".xml");
    if (filename != "")
    {
        openDatabase(filename);
    }
}

void ExplorerWindow::openDatabase(const QString &file)
{
    QFileInfo fi(file);
    if (fi.exists())
    {
        columns.clear();
        f.initialize(QApplication::applicationDirPath(), file, logger);

        useCurrentTime = true;
        dateTimeToUse = QDateTime::currentDateTimeUtc();
        fillFieldsCombo();
        fillOperatorsCombo();
        fillTreeCombo();
        fillSearchResultColumns();
        recordEditor = NULL;
        setWindowTitle(tr("eDoc - ") + fi.baseName());
    }
}

void ExplorerWindow::on_save(IRecordPtr record, bool isNew)
{
    if (isNew)
    {
        f.databaseEngine()->addRecord(record);
    }
    else
    {
        f.databaseEngine()->updateRecord(record);
    }
    f.tagEngine()->processRecord(record);
}

void ExplorerWindow::on_cancelEdition(IRecordPtr record)
{
}
