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
    m_Record = NULL;
}

RecordEditor::~RecordEditor()
{
    delete ui;
}

void RecordEditor::setRecord(IRecord * record)
{
    m_Record = record;
    ui->lstFields->clear();
    foreach (QString fieldName, record->fieldNames())
    {
        IFieldDefinition *fieldDef = record->fieldDefinition(fieldName);
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

QFieldWidget *RecordEditor::createWidget(IRecord *record, const QString &fieldName, QWidget* parent)
{
    QFieldWidget *w = NULL;
    QString fieldType = record->fieldDefinition(fieldName)->type();

    if (fieldType == "string")
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
        connect(dw, SIGNAL(download(const IValue*)), this, SLOT(download(const IValue*)));
        connect(dw, SIGNAL(upload(const IValue*)), this, SLOT(upload(const IValue*)));
    }
    return w;
}

void RecordEditor::applyValuesToRecord(IRecord *record)
{
    QStringList fields = collection.keys();
    foreach (QString fieldName, fields)
    {
        IFieldDefinition* fieldDef = record->fieldDefinition(fieldName);
        if (fieldDef && (fieldDef->isVisible() && fieldDef->isQueryable()))
        {
            QVariant v = collection[fieldName]->value();
            //record->value(fieldName)->setValue(v);
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

void RecordEditor::download(const IValue* value)
{
    emit downloadFile(m_Record, value);
}

void RecordEditor::upload(const IValue* value)
{
    emit uploadFile(m_Record, value);
}
