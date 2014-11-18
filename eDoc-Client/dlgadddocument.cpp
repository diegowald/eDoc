#include "dlgadddocument.h"
#include "ui_dlgadddocument.h"

DlgAddDocument::DlgAddDocument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddDocument)
{
    ui->setupUi(this);
    recEditor = NULL;
}

DlgAddDocument::~DlgAddDocument()
{
    delete ui;
}

void DlgAddDocument::setData(const QString & filename, QSharedPointer<IRecord> record)
{
    m_FileName = filename;
    m_Record = record;

    recEditor = new RecordEditor(this);
    recEditor->setRecord(record, true);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(recEditor);
    ui->frame->setLayout(layout);

    ui->lblFilename->setText(filename);
}

void DlgAddDocument::applyData(QSharedPointer<IRecord> record)
{
    recEditor->applyValuesToRecord(record);
}

QSharedPointer<IRecord> DlgAddDocument::record()
{
    return m_Record;
}

QString DlgAddDocument::filename()
{
    return m_FileName;
}
