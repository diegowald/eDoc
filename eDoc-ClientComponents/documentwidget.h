#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include "edoc-clientcomponents_global.h"
#include <QWidget>
#include "../eDoc-API/IValue.h"
#include "../eDoc-API/IFieldDefinition.h"
#include "QFieldWidget.h"

namespace Ui {
class DocumentWidget;
}

class EDOCCLIENTCOMPONENTSSHARED_EXPORT DocumentWidget : public QFieldWidget
{
    Q_OBJECT

public:
    explicit DocumentWidget(QWidget *parent = 0);
    ~DocumentWidget();

    void setField(IFieldDefinition* fieldDefinition, IValue* value);
    virtual QVariant value();

signals:
    void download(const IValue* value);
    void upload(const IValue* value);

private slots:
    void on_btnDownload_clicked();

    void on_btnupload_clicked();

private:
    Ui::DocumentWidget *ui;
    IFieldDefinition* m_FieldDefinition;
    IValue* m_Value;
};

#endif // DOCUMENTWIDGET_H
