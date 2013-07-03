#ifndef VALUEDEFINITIONS_H
#define VALUEDEFINITIONS_H

#include <QObject>
#include "value.h"
#include <QDateTime>
#include <QDate>
#include <QTime>
#include "edoc-metadataframework_global.h"

class EDOCMETADATAFRAMEWORKSHARED_EXPORT IntegerValue : public QObject, public Value<int>
{
    Q_OBJECT
public:
    explicit IntegerValue(int value, QObject *parent = 0);
    virtual ~IntegerValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT doubleValue : public QObject, public Value<double>
{
    Q_OBJECT
public:
    doubleValue(double value, QObject *parent = 0);
    virtual ~doubleValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT boolValue : public QObject, public Value<bool>
{
    Q_OBJECT
public:
    boolValue(bool value, QObject *parent = 0);
    virtual ~boolValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT QStringValue : public QObject, public Value<QString>
{
    Q_OBJECT
public:
    explicit QStringValue(const QString &value, QObject *parent = 0);
    virtual ~QStringValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT QDateTimeValue : public QObject, public Value<QDateTime>
{
    Q_OBJECT
public:
    explicit QDateTimeValue(const QDateTimeValue &value, QObject *parent = 0);
    virtual ~QDateTimeValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT QDateValue : public QObject, public Value<QDate>
{
    Q_OBJECT
public:
    explicit QDateValue(const QDateValue &value, QObject *parent = 0);
    virtual ~QDateValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT QTimeValue : public QObject, public Value<QDateTime>
{
    Q_OBJECT
public:
    explicit QTimeValue(const QTimeValue &value, QObject *parent = 0);
    virtual ~QTimeValue();
};


#endif // VALUEDEFINITIONS_H
