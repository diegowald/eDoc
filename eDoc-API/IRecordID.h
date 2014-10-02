#ifndef IRECORDID_H
#define IRECORDID_H

#include "forward.h"
#include <QMetaType>
#include <QSharedPointer>
#include "IMetadata.h"

class IRecordID
{
public:
    virtual QString asString() const = 0;
    virtual IMetadataPtr metadata() = 0;
    virtual ~IRecordID() {}
};


Q_DECLARE_METATYPE(IRecordIDPtr)

#endif // IRECORDID_H
