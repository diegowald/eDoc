#ifndef VALUE_H
#define VALUE_H

#include "../eDoc-API/IValue.h"
#include "edoc-metadataframework_global.h"
#include "fielddefinition.h"

template<class TYPE> class EDOCMETADATAFRAMEWORKSHARED_EXPORT Value : public IValue
{
public:
    Value(TYPE newValue)
    {
        m_Value = newValue;
        m_Null = false;
    }

    Value()
    {
        m_Null = true;
    }

    virtual ~Value() {}

    virtual void setValue(const QVariant &newValue) = 0;

    virtual QVariant asVariant() = 0;

    virtual QVariant content()
    {
        if (isNull())
            return QVariant(QVariant::String);
        else
            return asVariant();
    }

    virtual void setNull()
    {
        m_Null = true;
    }

    virtual bool isNull() { return m_Null; }

    virtual void setValue2(TYPE newValue)
    {
        m_Value = newValue;
        m_Null = false;
    }

    TYPE get()
    {
        return m_Value;
    }

    void setFieldDefinition(IFieldDefinition* fieldDef) { m_fieldDef = fieldDef; }
    IFieldDefinition* fieldDefinition() { return m_fieldDef; }

    // slots
    virtual void prepareToSave() {}
    virtual void prepareToLoad() {}

private:
    TYPE m_Value;
    bool m_Null;
    IFieldDefinition* m_fieldDef;
};

#endif // VALUE_H
