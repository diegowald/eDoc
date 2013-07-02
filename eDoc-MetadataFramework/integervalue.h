#ifndef INTEGERVALUE_H
#define INTEGERVALUE_H

#include <QObject>
#include "eDoc-API/IValue.h"

class IntegerValue : public QObject, public IValue
{
    Q_OBJECT
public:
    explicit IntegerValue(QObject *parent = 0);
    virtual ~IntegerValue();
    
signals:
    
public slots:
    
};

#endif // INTEGERVALUE_H
