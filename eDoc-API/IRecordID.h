#ifndef IRECORDID_H
#define IRECORDID_H

#include <QMetaType>
#include "IMetadata.h"

struct IRecordID
{
public:
    virtual QString asString() const = 0;
    virtual IMetadata *metadata() = 0;
    virtual ~IRecordID() {}
};

Q_DECLARE_METATYPE(IRecordID*)

#endif // IRECORDID_H
