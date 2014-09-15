#include "valuedefinitions.h"
#include "fielddefinition.h"
#include <QFile>

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
    setValue2(newValue.toTime());
}

QVariant QTimeValue::asVariant()
{
    return isNull() ? QVariant(QVariant::Time) : get();//.toString("hh:mm:ss");
}

IDocBaseValue::IDocBaseValue(IDocBasePtr value, IFieldDefinition *fieldDef, QObject *parent) :
    QObject(parent), Value(value)
{
    setFieldDefinition(fieldDef);
}

IDocBaseValue::~IDocBaseValue()
{
}

IDocumentValue::IDocumentValue(IDocumentPtr value, IFieldDefinition* fieldDef, QObject *parent) :
    QObject(parent), Value(value)
{
    this->setFieldDefinition(fieldDef);
}

void IDocumentValue::setValue(const QVariant &newValue)
{
    IDocBasePtr doc = newValue.value<IDocumentPtr>();
    if (doc.isNull())
    {
        IDocEnginePtr engine = ((FieldDefinition*)fieldDefinition())->getEngine();
        if (newValue.toString().startsWith("file:"))
        {
            filePath = newValue.toString().replace("filw:", "");
            QFile file(filePath);
            file.open(QIODevice::ReadOnly);
            QByteArray blob = file.readAll();
            doc = engine->createDocument(filePath, blob);
            file.close();
        }
        else
        {
            QString documentID = newValue.toString();
            doc = engine->getDocument(documentID);
        }
    }
    setValue2(doc.dynamicCast<IDocument>());
}

QVariant IDocumentValue::asVariant()
{
}

QVariant IDocumentValue::content()
{
    QVariant ret;

    if (isNull() && filePath.length() == 0)
    {
        ret.setValue(IDocumentPtr());
    }
    else
    {
        ret.setValue(get());
    }
    return ret;
}

// slots
void IDocumentValue::prepareToSave()
{
}

void IDocumentValue::prepareToLoad()
{
}


IDocumentValue::~IDocumentValue()
{
}


void IDocBaseValue::setValue(const QVariant &newValue)
{
    (void)newValue;
    // Esto es una exception
}

QVariant IDocBaseValue::asVariant()
{
    // esto es una exception
    return QVariant();
}

QVariant IDocBaseValue::content()
{
    if (isNull())
        return QVariant(QVariant::String);
    else
        return get()->id()->asString();
}

IDocumentIDValue::IDocumentIDValue(QSharedPointer<IDocID> value, QObject *parent) :
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

IMultiDocumentValue::IMultiDocumentValue(QSharedPointer<IMultiDocument> value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IMultiDocumentValue::~IMultiDocumentValue()
{
}

void IMultiDocumentValue::setValue(const QVariant &newValue)
{
    setValue2(qvariant_cast<QSharedPointer<IMultiDocument>>(newValue));
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

IRecordValue::IRecordValue(QSharedPointer<IRecord> value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IRecordValue::~IRecordValue()
{
}

void IRecordValue::setValue(const QVariant &newValue)
{
    setValue2(qvariant_cast<QSharedPointer<IRecord>>(newValue));
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

IMultiRecordValue::IMultiRecordValue(QSharedPointer<IMultiRecord> value, QObject *parent) :
    QObject(parent), Value(value)
{
}

IMultiRecordValue::~IMultiRecordValue()
{
}

void IMultiRecordValue::setValue(const QVariant &newValue)
{
    setValue2(qvariant_cast<QSharedPointer<IMultiRecord>>(newValue));
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
