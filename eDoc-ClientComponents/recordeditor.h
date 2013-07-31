#ifndef RECORDEDITOR_H
#define RECORDEDITOR_H

#include "edoc-clientcomponents_global.h"
#include <QWidget>
#include "../eDoc-API/IRecord.h"
#include "stringwidget.h"

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
private:
    Ui::RecordEditor *ui;
    QMap<QString, StringWidget*> collection;
    bool enabledEdition;
};

#endif // RECORDEDITOR_H
