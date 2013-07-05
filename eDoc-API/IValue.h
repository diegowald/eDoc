#ifndef IVALUE_H
#define IVALUE_H

struct IValue
{
public:
    virtual void setValue(const QString &newValue) = 0;
    virtual QString asString() = 0;
    virtual void setNull() = 0;
    virtual bool isNull() = 0;
    virtual ~IValue() {}
};

#endif // IVALUE_H
