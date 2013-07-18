#ifndef IVALUE_H
#define IVALUE_H
#include <QVariant>

struct IValue
{
public:
    //virtual void setValue(const QString &newValue) = 0;
    virtual void setValue(const QVariant &newValue) = 0;
    //virtual QString asString() = 0;
    virtual QVariant asVariant() = 0;
    virtual QVariant content() = 0;
    virtual void setNull() = 0;
    virtual bool isNull() = 0;
    virtual ~IValue() {}
};

#endif // IVALUE_H
