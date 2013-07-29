#ifndef DLGADDDOCUMENT_H
#define DLGADDDOCUMENT_H

#include <QDialog>
#include "../eDoc-API/IRecord.h"
#include "../eDoc-ClientComponents/recordeditor.h"

namespace Ui {
class DlgAddDocument;
}

class DlgAddDocument : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgAddDocument(QWidget *parent = 0);
    ~DlgAddDocument();

    void setData(const QString &filename, IRecord* record);
    void applyData(IRecord *record);
    IRecord* record();
    QString filename();


private:
    Ui::DlgAddDocument *ui;

    QString m_FileName;
    IRecord *m_Record;
    RecordEditor *recEditor;
};

#endif // DLGADDDOCUMENT_H
