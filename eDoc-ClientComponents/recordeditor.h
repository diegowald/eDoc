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

    virtual void setRecord(IRecordPtr record);
    virtual void applyValuesToRecord(IRecordPtr record);
    virtual void setEnabledEdition(bool enabled);

private:
    void setEnabledStatus();
    QFieldWidget *createWidget(IRecordPtr record, const QString &fieldName, QWidget* parent);
    void setButtonsState();
    void applyValuesToRecord();
signals:
    void downloadFile(IRecordPtr record, const IValuePtr value);
    void uploadFile(IRecordPtr record, const IValuePtr value, QString &fileLocation);
    void save(IRecordPtr record);
    void cancelEdition(IRecordPtr record);

private slots:
    void download(const IValuePtr value);
    void upload(const IValuePtr value, QString &fileLocation);
    void on_btnCancel_pressed();
    void on_btnSave_pressed();
    void on_fieldChanged(IValuePtr valueChanged);
    void setChanged();
    void setUnchanged();

private:
    Ui::RecordEditor *ui;
    QMap<QString, QFieldWidget*> collection;
    bool enabledEdition;
    QSharedPointer<IRecord> m_Record;
    bool recordChanged;
};

#endif // RECORDEDITOR_H
