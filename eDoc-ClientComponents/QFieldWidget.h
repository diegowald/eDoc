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

    virtual void setField(IFieldDefinitionPtr fieldDefinition, IValuePtr value)
    {
        m_FieldDefinition = fieldDefinition;
        m_Value = value;
    }

    virtual QVariant value()
    {
        return m_Value->asVariant();
    }

signals:
    void fieldChanged(IValuePtr valueChanged);

protected:
    IFieldDefinitionPtr m_FieldDefinition;
    IValuePtr m_Value;
};

#endif // QFIELDWIDGET_H
