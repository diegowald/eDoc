#ifndef IRECORDID_H
#define IRECORDID_H

#include <QMetaType>
#include <QSharedPointer>
#include "IMetadata.h"

struct IRecordID
{
public:
    virtual QString asString() const = 0;
    virtual IMetadataPtr metadata() = 0;
    virtual ~IRecordID() {}
};

typedef QSharedPointer<IRecordID> IRecordIDPtr;
Q_DECLARE_METATYPE(IRecordIDPtr)

#endif // IRECORDID_H
