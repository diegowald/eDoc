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
    setUnchanged();
    _isNew = false;
}

RecordEditor::~RecordEditor()
{
    delete ui;
}

void RecordEditor::setRecord(IRecordPtr record, bool isNew)
{
    _isNew = isNew;
    m_Record = record;
    setUnchanged();
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
            connect(w, SIGNAL(fieldChanged(IValuePtr)), this, SLOT(on_fieldChanged(IValuePtr)));
        }
    }
    setEnabledStatus();
}

QFieldWidget *RecordEditor::createWidget(IRecordPtr record, const QString &fieldName, QWidget* parent)
{
    QFieldWidget *w = NULL;

    switch (record->fieldDefinition(fieldName)->type())
    {
    case DATATYPE::QSTRING_TYPE:
    {
        StringWidget *sw = new StringWidget(parent);
        sw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
        w = sw;
        break;
    }
    case DATATYPE::INTEGER_TYPE:
    {
        StringWidget *sw = new StringWidget(parent);
        sw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
        w = sw;
        break;
    }
    case DATATYPE::TAG_TYPE:
    {
        StringWidget *sw = new StringWidget(parent);
        sw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
        w = sw;
        break;
    }
    case DATATYPE::IDOCUMENT_TYPE:
    {
        DocumentWidget *dw = new DocumentWidget(parent);
        dw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
        w = dw;
        connect(dw, SIGNAL(download(const IValuePtr)), this, SLOT(download(const IValuePtr)));
        connect(dw, SIGNAL(upload(const IValuePtr,QString &)), this, SLOT(upload(const IValuePtr,QString &)));
        break;
    }
    default:
        break;
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

void RecordEditor::applyValuesToRecord()
{
    applyValuesToRecord(m_Record);
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

void RecordEditor::upload(const IValuePtr value, QString &fileLocation)
{

    emit uploadFile(m_Record, value, fileLocation);
}

void RecordEditor::on_btnCancel_pressed()
{
    emit cancelEdition(m_Record);
    setUnchanged();
}

void RecordEditor::on_btnSave_pressed()
{
    if (recordChanged)
    {
        applyValuesToRecord();
        emit save(m_Record, _isNew);
        setUnchanged();
    }
}

void RecordEditor::on_fieldChanged(IValuePtr valueChanged)
{
    setChanged();
}

void RecordEditor::setUnchanged()
{
    recordChanged = false;
    setButtonsState();
}

void RecordEditor::setChanged()
{
    recordChanged = true;
    setButtonsState();
}

void RecordEditor::setButtonsState()
{
    ui->btnSave->setEnabled(recordChanged);
    ui->btnCancel->setEnabled(!recordChanged);
}
