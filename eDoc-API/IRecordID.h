#ifndef IRECORDID_H
#define IRECORDID_H

#include <QMetaType>
#include <QSharedPointer>
#include "IMetadata.h"

struct IRecordID
{
public:
    virtual QString asString() const = 0;
    virtual QSharedPointer<IMetadata> metadata() = 0;
    virtual ~IRecordID() {}
};

Q_DECLARE_METATYPE(QSharedPointer<IRecordID>)

#endif // IRECORDID_H
