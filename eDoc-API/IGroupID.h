#ifndef IGROUPID_H
#define IGROUPID_H

struct IGroupID
{
public:
    virtual QString asString() const = 0;
    virtual ~IGroupID() {}
};

#endif // IGROUPID_H
