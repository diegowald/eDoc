#ifndef VALUE_H
#define VALUE_H

#include "../eDoc-API/IValue.h"
#include "edoc-metadataframework_global.h"

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


    virtual void setValue(const QString &newValue) = 0;

    virtual QString asString() = 0;

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

    /*Value( const Value& other )
    {
        m_Value = other.m_Value;
    }

    Value( Value& other )
    {
        m_Value = other.m_Value;
    }

    Value& operator=(const Value& rhs)
    {
        m_Value = rhs.m_Value;
        return *this;
    }*/

    TYPE get()
    {
        return m_Value;
    }

private:
    TYPE m_Value;
    bool m_Null;
};

#endif // VALUE_H
