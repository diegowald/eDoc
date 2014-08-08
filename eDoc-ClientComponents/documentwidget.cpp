#include "documentwidget.h"
#include "ui_documentwidget.h"

DocumentWidget::DocumentWidget(QWidget *parent) :
    QFieldWidget(parent),
    ui(new Ui::DocumentWidget)
{
    ui->setupUi(this);
    m_FieldDefinition = NULL;
    m_Value = NULL;
}

DocumentWidget::~DocumentWidget()
{
    delete ui;
}

void DocumentWidget::setField(IFieldDefinition* fieldDefinition, IValue* value)
{
//    ui->label->setText(fieldDefinition->name());
//    ui->ReadOnlyValue->setVisible(fieldDefinition->isReadOnly());
//    ui->EditValue->setVisible(!fieldDefinition->isReadOnly());
//    ui->ReadOnlyValue->setText(value->content().toString());
//    ui->EditValue->setText(value->content().toString());
    m_FieldDefinition = fieldDefinition;
    m_Value = value;
}

QVariant DocumentWidget::value()
{
    return m_Value->asVariant();
}

void DocumentWidget::on_btnDownload_clicked()
{
    if (m_Value != NULL)
    {
        emit download(m_Value);
    }
}

void DocumentWidget::on_btnupload_clicked()
{
    if (!m_FieldDefinition->isReadOnly())
    {
        emit upload(m_Value);
    }
}
