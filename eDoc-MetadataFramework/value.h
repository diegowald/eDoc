#ifndef VALUE_H
#define VALUE_H

#include "../eDoc-API/IValue.h"

template<class TYPE> class Value : public IValue
{
public:
    Value(TYPE newValue)
    {
        m_Value = newValue;
    }

    virtual ~Value() {}

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
};

#endif // VALUE_H
