#include "valuedefinitions.h"

IntegerValue::IntegerValue(int value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IntegerValue::IntegerValue(QObject *parent) :
    QObject(parent), Value()
{
}

void IntegerValue::setValue(const QVariant &newValue)
{
    setValue2(newValue.toInt());
}

QVariant IntegerValue::asVariant()
{
    return isNull() ? QVariant(QVariant::Int) : get();
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

void DoubleValue::setValue(const QVariant &newValue)
{
    setValue2(newValue.toDouble());
}

QVariant DoubleValue::asVariant()
{
    return isNull() ? QVariant(QVariant::Double) : get();
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

void BoolValue::setValue(const QVariant &newValue)
{
    setValue2("1" == newValue);
}

QVariant BoolValue::asVariant()
{
    return isNull() ? QVariant(QVariant::Bool) : (get() ? true : false);
}


QStringValue::QStringValue(const QString &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QStringValue::QStringValue(QObject *parent) :
    QObject(parent), Value()
{
}

void QStringValue::setValue(const QVariant &newValue)
{
    setValue2(newValue.toString());
}

QVariant QStringValue::asVariant()
{
    return isNull() ? QVariant(QVariant::String) : get();
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

void QDateTimeValue::setValue(const QVariant &newValue)
{
    //setValue2(QDateTime::fromString(newValue, "yyyyMMdd hh:mm:ss"));
    setValue2(newValue.toDateTime());
}

QVariant QDateTimeValue::asVariant()
{
    return isNull() ? QVariant(QVariant::DateTime) : get();//.toString("yyyyMMdd hh:mm:ss");
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

void QDateValue::setValue(const QVariant &newValue)
{
    //setValue2(QDate::fromString(newValue, "yyyyMMdd"));
    setValue2(newValue.toDate());
}

QVariant QDateValue::asVariant()
{
    return isNull() ? QVariant(QVariant::Date) : get();//.toString("yyyyMMdd");
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

void QTimeValue::setValue(const QVariant &newValue)
{
    //setValue2(QTime::fromString(newValue, "hh:mm:ss"));
    setValue2(newValue.toTime());
}

QVariant QTimeValue::asVariant()
{
    return isNull() ? QVariant(QVariant::Time) : get();//.toString("hh:mm:ss");
}

IDocBaseValue::IDocBaseValue(IDocBase *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IDocBaseValue::~IDocBaseValue()
{
}

void IDocBaseValue::setValue(const QVariant &newValue)
{
    // Esto es una exception
}

QVariant IDocBaseValue::asVariant()
{
    // esto es una exception
}

QVariant IDocBaseValue::content()
{
    if (isNull())
        return QVariant(QVariant::String);
    else
        return get()->id()->asString();
}

IDocumentIDValue::IDocumentIDValue(IDocID *value, QObject *parent) :
    QObject(parent), Value(value ? value->asString() : QString())
{
}

IDocumentIDValue::IDocumentIDValue(const QString &value, QObject *parent) :
    QObject(parent), Value(value)
{
}


IDocumentIDValue::~IDocumentIDValue()
{
}

void IDocumentIDValue::setValue(const QVariant &newValue)
{
    // Esto es una exception
    setValue2(newValue.toString());
}

QVariant IDocumentIDValue::asVariant()
{
    return qVariantFromValue(get());
}

QVariant IDocumentIDValue::content()
{
    if (isNull())
        return QVariant(QVariant::String);
    else
        return get();
}

IMultiDocumentValue::IMultiDocumentValue(IMultiDocument *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IMultiDocumentValue::~IMultiDocumentValue()
{
}

void IMultiDocumentValue::setValue(const QVariant &newValue)
{
    setValue2(qvariant_cast<IMultiDocument*>(newValue));
}

QVariant IMultiDocumentValue::asVariant()
{
    QVariant v;
    v.setValue(get());
    return v;
}

QVariant IMultiDocumentValue::content()
{
    if (isNull())
        return QVariant(QVariant::String);
    else
        return get()->id()->asString();
}

IRecordValue::IRecordValue(IRecord *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IRecordValue::~IRecordValue()
{
}

void IRecordValue::setValue(const QVariant &newValue)
{
    setValue2(qvariant_cast<IRecord*>(newValue));
}

QVariant IRecordValue::asVariant()
{
    QVariant v;
    v.setValue(get());
    return v;
}

QVariant IRecordValue::content()
{
    if (isNull())
        return QVariant(QVariant::String);
    else
        return get()->ID()->asString();
}

IMultiRecordValue::IMultiRecordValue(IMultiRecord *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IMultiRecordValue::~IMultiRecordValue()
{
}

void IMultiRecordValue::setValue(const QVariant &newValue)
{
    setValue2(qvariant_cast<IMultiRecord*>(newValue));
}

QVariant IMultiRecordValue::asVariant()
{
    QVariant v;
    v.setValue(get());
    return v;
}

QVariant IMultiRecordValue::content()
{
    if (isNull())
        return QVariant(QVariant::String);
    else
        return get()->ID()->asString();
}

/*
ITagRecordValue::ITagRecordValue(ITag *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

ITagRecordValue::~ITagRecordValue()
{
}

void ITagRecordValue::setValue(const QVariant &newValue)
{
    setValue2(qvariant_cast<ITag*>(newValue));
}

QVariant ITagRecordValue::asVariant()
{
    QVariant v;
    v.setValue(get());
    return v;
}

QVariant ITagRecordValue::content()
{
    if (isNull())
        return QVariant(QVariant::String);
    else
        return get()->asString();
}
*/
