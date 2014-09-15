#ifndef STRINGWIDGET_H
#define STRINGWIDGET_H

#include "edoc-clientcomponents_global.h"
#include "../eDoc-API/IValue.h"
#include "../eDoc-API/IFieldDefinition.h"
#include "QFieldWidget.h"

namespace Ui {
class StringWidget;
}

class EDOCCLIENTCOMPONENTSSHARED_EXPORT StringWidget : public QFieldWidget
{
    Q_OBJECT
    
public:
    explicit StringWidget(QWidget *parent = 0);
    ~StringWidget();

    virtual void setField(QSharedPointer<IFieldDefinition> fieldDefinition, QSharedPointer<IValue> value);
    virtual QVariant value();
private slots:
    void on_EditValue_textChanged(const QString &arg1);

private:
    Ui::StringWidget *ui;
};

#endif // STRINGWIDGET_H
