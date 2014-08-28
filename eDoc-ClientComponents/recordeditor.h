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

    virtual void setRecord(QSharedPointer<IRecord> record);
    virtual void applyValuesToRecord(QSharedPointer<IRecord> record);
    virtual void setEnabledEdition(bool enabled);

private:
    void setEnabledStatus();
    QFieldWidget *createWidget(QSharedPointer<IRecord> record, const QString &fieldName, QWidget* parent);

signals:
    void downloadFile(QSharedPointer<IRecord> record, const QSharedPointer<IValue> value);
    void uploadFile(QSharedPointer<IRecord> record, const QSharedPointer<IValue> value);

private slots:
    void download(const QSharedPointer<IValue> value);
    void upload(const QSharedPointer<IValue> value);

private:
    Ui::RecordEditor *ui;
    QMap<QString, QFieldWidget*> collection;
    bool enabledEdition;
    QSharedPointer<IRecord> m_Record;
};

#endif // RECORDEDITOR_H
