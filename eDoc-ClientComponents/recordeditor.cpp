#include "recordeditor.h"
#include "ui_recordeditor.h"
#include "stringwidget.h"

RecordEditor::RecordEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordEditor)
{
    ui->setupUi(this);
}

RecordEditor::~RecordEditor()
{
    delete ui;
}

void RecordEditor::setRecord(IRecord * record)
{
    ui->lstFields->clear();
    foreach (QString fieldName, record->fieldNames())
    {
        if (record->fieldDefinition(fieldName)->isVisible())
        {
            QListWidgetItem *item = new QListWidgetItem("");
            ui->lstFields->addItem(item);
            StringWidget *sw = new StringWidget(ui->lstFields);
            ui->lstFields->setItemWidget(item, sw);
            item->setSizeHint(sw->size());
            sw->setField(record->fieldDefinition(fieldName), record->value(fieldName));
            collection[fieldName] = sw;
        }
    }
}

void RecordEditor::applyValuesToRecord(IRecord *record)
{
    ui->lstFields->clear();
    foreach (QString fieldName, record->fieldNames())
    {
        if (record->fieldDefinition(fieldName)->isVisible())
        {
            record->value(fieldName)->setValue(collection[fieldName]->value());
        }
    }
}
