#ifndef IRECORDID_H
#define IRECORDID_H

struct IRecordID
{
public:
    virtual QString asString() const = 0;
    virtual ~IRecordID() {}
};


#endif // IRECORDID_H
