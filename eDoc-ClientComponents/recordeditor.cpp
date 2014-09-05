#include "recordeditor.h"
#include "ui_recordeditor.h"
#include "stringwidget.h"
#include "documentwidget.h"

RecordEditor::RecordEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordEditor)
{
    ui->setupUi(this);
    enabledEdition = true;
}

RecordEditor::~RecordEditor()
{
    delete ui;
}

void RecordEditor::setRecord(IRecordPtr record)
{
    m_Record = record;
    ui->lstFields->clear();
    foreach (QString fieldName, record->fieldNames())
    {
        QSharedPointer<IFieldDefinition> fieldDef = record->fieldDefinition(fieldName);
        if (fieldDef->isVisible())
        {
            QFieldWidget *w = createWidget(record, fieldName, ui->lstFields);
            if (w != NULL)
            {
                QListWidgetItem *item = new QListWidgetItem("");
                ui->lstFields->addItem(item);
                ui->lstFields->setItemWidget(item, w);
                item->setSizeHint(w->size());
                collection[fieldName] = w;
            }
        }
    }
    setEnabledStatus();
}

QFieldWidget *RecordEditor::createWidget(QSharedPointer<IRecord> record, const QString &fieldName, QWidget* parent)
{
    QFieldWidget *w = NULL;
    QString fieldType = record->fieldDefinition(fieldName)->type();

    if (fieldType == "string")
    {
        StringWidget *sw = new StringWidget(parent);
        sw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
        w = sw;
    }
    else if (fieldType == "integer")
    {
        StringWidget *sw = new StringWidget(parent);
        sw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
        w = sw;
    }
    else if (fieldType == "tag")
    {
        StringWidget *sw = new StringWidget(parent);
        sw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
        w = sw;
    }
    else if (fieldType == "document")
    {
        DocumentWidget *dw = new DocumentWidget(parent);
        dw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
        w = dw;
        connect(dw, SIGNAL(download(const IValuePtr)), this, SLOT(download(const IValuePtr)));
        connect(dw, SIGNAL(upload(const IValuePtr)), this, SLOT(upload(const IValuePtr)));
    }
    return w;
}

void RecordEditor::applyValuesToRecord(IRecordPtr record)
{
    QStringList fields = collection.keys();
    foreach (QString fieldName, fields)
    {
        QSharedPointer<IFieldDefinition> fieldDef = record->fieldDefinition(fieldName);
        if (fieldDef && (fieldDef->isVisible() && fieldDef->isQueryable()))
        {
            QVariant v = collection[fieldName]->value();
            record->value(fieldName)->setValue(collection[fieldName]->value());
        }
    }
    fields.clear();
}

void RecordEditor::setEnabledEdition(bool enabled)
{
    enabledEdition = enabled;
    setEnabledStatus();
}


void RecordEditor::setEnabledStatus()
{
    foreach (QFieldWidget* w, collection.values())
    {
        w->setEnabled(enabledEdition);
    }
}

void RecordEditor::download(const IValuePtr value)
{
    emit downloadFile(m_Record, value);
}

void RecordEditor::upload(const IValuePtr value)
{
    emit uploadFile(m_Record, value);
}
