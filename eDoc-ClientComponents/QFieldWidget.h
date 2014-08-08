#ifndef QFIELDWIDGET_H
#define QFIELDWIDGET_H

#include <QWidget>
#include "edoc-clientcomponents_global.h"
#include "../eDoc-API/IValue.h"
#include "../eDoc-API/IFieldDefinition.h"


class EDOCCLIENTCOMPONENTSSHARED_EXPORT QFieldWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QFieldWidget(QWidget *parent = 0) : QWidget(parent) {}
    virtual ~QFieldWidget(){}

    virtual void setField(IFieldDefinition* fieldDefinition, IValue* value) {}
    virtual QVariant value() {return QVariant();}
};

#endif // QFIELDWIDGET_H
