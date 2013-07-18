#ifndef VALUEDEFINITIONS_H
#define VALUEDEFINITIONS_H

#include <QObject>
#include "value.h"
#include <QDateTime>
#include <QDate>
#include <QTime>
#include "edoc-metadataframework_global.h"

#include "../eDoc-API/IDocBase.h"
#include "../eDoc-API/IDocument.h"
#include "../eDoc-API/IMultiDocument.h"

class EDOCMETADATAFRAMEWORKSHARED_EXPORT IntegerValue : public QObject, public Value<int>
{
    Q_OBJECT
public:
    explicit IntegerValue(int value, QObject *parent = 0);
    IntegerValue(QObject *parent = 0);
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();

    virtual ~IntegerValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT DoubleValue : public QObject, public Value<double>
{
    Q_OBJECT
public:
    DoubleValue(double value, QObject *parent = 0);
    DoubleValue(QObject *parent = 0);
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual ~DoubleValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT BoolValue : public QObject, public Value<bool>
{
    Q_OBJECT
public:
    BoolValue(bool value, QObject *parent = 0);
    BoolValue(QObject *parent = 0);
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual ~BoolValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT QStringValue : public QObject, public Value<QString>
{
    Q_OBJECT
public:
    explicit QStringValue(const QString &value, QObject *parent = 0);
    QStringValue(QObject *parent = 0);
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual ~QStringValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT QDateTimeValue : public QObject, public Value<QDateTime>
{
    Q_OBJECT
public:
    explicit QDateTimeValue(const QDateTimeValue &value, QObject *parent = 0);
    QDateTimeValue(QObject *parent = 0);
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual ~QDateTimeValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT QDateValue : public QObject, public Value<QDate>
{
    Q_OBJECT
public:
    explicit QDateValue(const QDateValue &value, QObject *parent = 0);
    QDateValue(QObject *parent = 0);
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual ~QDateValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT QTimeValue : public QObject, public Value<QTime>
{
    Q_OBJECT
public:
    explicit QTimeValue(const QTimeValue &value, QObject *parent = 0);
    QTimeValue(QObject *parent = 0);
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual ~QTimeValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT IDocBaseValue : public QObject, public Value<IDocBase*>
{
    Q_OBJECT
public:
    IDocBaseValue(IDocBase * value, QObject *parent = 0);
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual QVariant content();
    virtual ~IDocBaseValue();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT IDocumentValue : public QObject, public Value<IDocument*>
{
    Q_OBJECT
public:
    IDocumentValue(IDocument *value, QObject *parent = 0);
    virtual ~IDocumentValue();
    virtual void setValue(const QVariant &newValue);
    virtual QVariant content();
    virtual QVariant asVariant();
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT IMultiDocumentValue : public QObject, public Value<IMultiDocument*>
{
    Q_OBJECT
public:
    IMultiDocumentValue(IMultiDocument *value, QObject *parent = 0);
    virtual ~IMultiDocumentValue();
    virtual void setValue(const QVariant &newValue);
    virtual QVariant content();
    virtual QVariant asVariant();
};

#endif // VALUEDEFINITIONS_H
