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

void DlgAddDocument::setData(const QString &filename, IRecord* record)
{
    m_FileName = filename;
    m_Record = record;

    recEditor = new RecordEditor(this);
    recEditor->setRecord(record);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(recEditor);
    ui->frame->setLayout(layout);

    ui->lblFilename->setText(m_FileName);
}

void DlgAddDocument::applyData(IRecord *record)
{
    recEditor->applyValuesToRecord(record);
}

IRecord* DlgAddDocument::record()
{
    return m_Record;
}

QString DlgAddDocument::filename()
{
    return m_FileName;
}