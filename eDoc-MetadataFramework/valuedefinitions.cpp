#include "valuedefinitions.h"

IntegerValue::IntegerValue(int value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IntegerValue::~IntegerValue()
{
}

doubleValue::doubleValue(double value, QObject *parent) :
    QObject(parent), Value(value)
{
}

doubleValue::~doubleValue()
{
}

boolValue::boolValue(bool value, QObject *parent) :
    QObject(parent), Value(value)
{
}

boolValue::~boolValue()
{
}

QStringValue::QStringValue(const QString &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QStringValue::~QStringValue()
{
}

QDateTimeValue::QDateTimeValue(const QDateTimeValue &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QDateTimeValue::~QDateTimeValue()
{
}

QDateValue::QDateValue(const QDateValue &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QDateValue::~QDateValue()
{
}

QTimeValue::QTimeValue(const QTimeValue &value, QObject *parent) :
    QObject(parent), Value(value)
{
}

QTimeValue::~QTimeValue()
{
}

IDocBaseValue::IDocBaseValue(IDocBase *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IDocBaseValue::~IDocBaseValue()
{
}

IDocumentValue::IDocumentValue(IDocument *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IDocumentValue::~IDocumentValue()
{
}

IMultiDocumentValue::IMultiDocumentValue(IMultiDocument *value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IMultiDocumentValue::~IMultiDocumentValue()
{
}
