#include "documentwidget.h"
#include "ui_documentwidget.h"

DocumentWidget::DocumentWidget(QWidget *parent) :
    QFieldWidget(parent), ui(new Ui::DocumentWidget)
{
    ui->setupUi(this);
    _fileLocation = "";
}

DocumentWidget::~DocumentWidget()
{
    delete ui;
}

void DocumentWidget::setField(IFieldDefinitionPtr fieldDefinition, IValuePtr value)
{
    m_FieldDefinition = fieldDefinition;
    m_Value = value;
    _fileLocation = "";
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
        QString fileLocation = "";
        emit upload(m_Value, fileLocation);
        if (_fileLocation != fileLocation)
        {
            _fileLocation = fileLocation;
            m_Value->setValue("file:" + fileLocation);
            emit fieldChanged(m_Value);
        }
    }
}
