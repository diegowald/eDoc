#include "valuedefinitions.h"

IntegerValue::IntegerValue(int value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IntegerValue::IntegerValue(QObject *parent) :
    QObject(parent), Value()
{
}

void IntegerValue::setValue(const QString &newValue)
{
    setValue2(newValue.toInt());
}

QString IntegerValue::asString()
{
    return isNull() ? "" : QString::number(get());
}

IntegerValue::~IntegerValue()
{
}

DoubleValue::DoubleValue(double value, QObject *parent) :
    QObject(parent), Value(value)
{
}

DoubleValue::DoubleValue(QObject *parent) :
    QObject(parent), Value()
{
}

DoubleValue::~DoubleValue()
{
}

void DoubleValue::setValue(const QString &newValue)
{
    setValue2(newValue.toDouble());
}

QString DoubleValue::asString()
{
    return isNull() ? "" : QString::number(get());
}


BoolValue::BoolValue(bool value, QObject *parent) :
    QObject(parent), Value(value)
{
}

BoolValue::BoolValue(QObject *parent) :
    QObject(parent), Value()
{
}

BoolValue::~BoolValue()
{
}

void BoolValue::setValue(const QString &newValue)
{
    setValue2("1" == newValue);
}

QString BoolValue::asString()
{
    return isNull() ? "" : (get() ? "1" : "0");
}


QStringValue::QStringValue(const QString &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QStringValue::QStringValue(QObject *parent) :
    QObject(parent), Value()
{
}

void QStringValue::setValue(const QString &newValue)
{
    setValue2(newValue);
}

QString QStringValue::asString()
{
    return isNull() ? "" : get();
}


QStringValue::~QStringValue()
{
}

QDateTimeValue::QDateTimeValue(const QDateTimeValue &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QDateTimeValue::QDateTimeValue(QObject *parent) :
    QObject(parent), Value()
{
}

void QDateTimeValue::setValue(const QString &newValue)
{
    setValue2(QDateTime::fromString(newValue, "yyyyMMdd hh:mm:ss"));
}

QString QDateTimeValue::asString()
{
    return isNull() ? "" : get().toString("yyyyMMdd hh:mm:ss");
}

QDateTimeValue::~QDateTimeValue()
{
}

QDateValue::QDateValue(const QDateValue &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QDateValue::QDateValue(QObject *parent) :
    QObject(parent), Value()
{
}

QDateValue::~QDateValue()
{
}

void QDateValue::setValue(const QString &newValue)
{
    setValue2(QDate::fromString(newValue, "yyyyMMdd"));
}

QString QDateValue::asString()
{
    return isNull() ? "" : get().toString("yyyyMMdd");
}


QTimeValue::QTimeValue(const QTimeValue &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QTimeValue::QTimeValue(QObject *parent) :
    QObject(parent), Value()
{
}

QTimeValue::~QTimeValue()
{
}

void QTimeValue::setValue(const QString &newValue)
{
    setValue2(QTime::fromString(newValue, "hh:mm:ss"));
}

QString QTimeValue::asString()
{
    return isNull() ? "" : get().toString("hh:mm:ss");
}

IDocBaseValue::IDocBaseValue(IDocBase *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IDocBaseValue::~IDocBaseValue()
{
}

void IDocBaseValue::setValue(const QString &newValue)
{
    // Esto es una exception
}

QString IDocBaseValue::asString()
{
    // esto es una exception
}

IDocumentValue::IDocumentValue(IDocument *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IDocumentValue::~IDocumentValue()
{
}

void IDocumentValue::setValue(const QString &newValue)
{
    // Esto es una exception
}

QString IDocumentValue::asString()
{
    // esto es una exception
}

IMultiDocumentValue::IMultiDocumentValue(IMultiDocument *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IMultiDocumentValue::~IMultiDocumentValue()
{
}

void IMultiDocumentValue::setValue(const QString &newValue)
{
    // Esto es una exception
}

QString IMultiDocumentValue::asString()
{
    // esto es una exception
}
