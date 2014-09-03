#include "documentwidget.h"
#include "ui_documentwidget.h"

DocumentWidget::DocumentWidget(QWidget *parent) :
    QFieldWidget(parent),
    ui(new Ui::DocumentWidget)
{
    ui->setupUi(this);
}

DocumentWidget::~DocumentWidget()
{
    delete ui;
}

void DocumentWidget::setField(QSharedPointer<IFieldDefinition> fieldDefinition, QSharedPointer<IValue> value)
{
    m_FieldDefinition = fieldDefinition;
    m_Value = value;
}

QVariant DocumentWidget::value()
{
    return m_Value->asVariant();
}

void DocumentWidget::on_btnDownload_clicked()
{
    if (!m_Value.isNull())
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
