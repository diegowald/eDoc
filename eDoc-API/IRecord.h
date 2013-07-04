#ifndef IRECORD_H
#define IRECORD_H
#include "IRecordID.h"

struct IRecord
{
public:
    virtual void setID(IRecordID *ID) = 0;
    virtual IRecordID *ID() = 0;

    virtual ~IRecord() {}
};

#endif // IRECORD_H
