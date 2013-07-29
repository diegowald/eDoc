#ifndef STRINGWIDGET_H
#define STRINGWIDGET_H

#include "edoc-clientcomponents_global.h"
#include <QWidget>
#include "../eDoc-API/IValue.h"
#include "../eDoc-API/IFieldDefinition.h"
namespace Ui {
class StringWidget;
}

class EDOCCLIENTCOMPONENTSSHARED_EXPORT StringWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StringWidget(QWidget *parent = 0);
    ~StringWidget();

    virtual void setField(IFieldDefinition* fieldDefinition, IValue* value);
    virtual QVariant value();
private:
    Ui::StringWidget *ui;
};

#endif // STRINGWIDGET_H
