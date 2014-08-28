#ifndef ITAG_H
#define ITAG_H

#include <QMetaType>
#include <QStringList>
#include <QSharedPointer>
#include "IValue.h"

struct ITag : public IValue
{
    //virtual void setValue(const QString &newValue) = 0;
    virtual void setValue(const QVariant &newValue) = 0;
    //virtual QString asString() = 0;
    virtual QVariant asVariant() = 0;
    virtual QVariant content() = 0;
    virtual void setNull() = 0;
    virtual bool isNull() = 0;
    virtual QStringList keys() = 0;
    virtual QString asString() = 0;
    virtual ~ITag() {}
};

Q_DECLARE_METATYPE(QSharedPointer<ITag>)

#endif // ITAG_H
