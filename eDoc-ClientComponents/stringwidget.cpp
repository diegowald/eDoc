#include "stringwidget.h"
#include "ui_stringwidget.h"

StringWidget::StringWidget(QWidget *parent) :
    QFieldWidget(parent),
    ui(new Ui::StringWidget)
{
    ui->setupUi(this);
}

StringWidget::~StringWidget()
{
    delete ui;
}

void StringWidget::setField(QSharedPointer<IFieldDefinition> fieldDefinition, QSharedPointer<IValue> value)
{
    ui->label->setText(fieldDefinition->name());
    ui->ReadOnlyValue->setVisible(fieldDefinition->isReadOnly());
    ui->EditValue->setVisible(!fieldDefinition->isReadOnly());
    ui->ReadOnlyValue->setText(value->content().toString());
    ui->EditValue->setText(value->content().toString());
}

QVariant StringWidget::value()
{
    return ui->EditValue->text();
}

void StringWidget::on_EditValue_textChanged(const QString &arg1)
{
    emit fieldChanged(m_Value);
}
