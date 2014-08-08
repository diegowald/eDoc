#ifndef RECORDEDITOR_H
#define RECORDEDITOR_H

#include "edoc-clientcomponents_global.h"
#include <QWidget>
#include "../eDoc-API/IRecord.h"
#include "QFieldWidget.h"

namespace Ui {
class RecordEditor;
}

class EDOCCLIENTCOMPONENTSSHARED_EXPORT RecordEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit RecordEditor(QWidget *parent = 0);
    virtual ~RecordEditor();

    virtual void setRecord(IRecord * record);
    virtual void applyValuesToRecord(IRecord *record);
    virtual void setEnabledEdition(bool enabled);

private:
    void setEnabledStatus();
    QFieldWidget *createWidget(IRecord *record, const QString &fieldName, QWidget* parent);

signals:
    void downloadFile(IRecord* record, const IValue* value);
    void uploadFile(IRecord* record, const IValue* value);

private slots:
    void download(const IValue* value);
    void upload(const IValue* value);

private:
    Ui::RecordEditor *ui;
    QMap<QString, QFieldWidget*> collection;
    bool enabledEdition;
    IRecord* m_Record;
};

#endif // RECORDEDITOR_H
