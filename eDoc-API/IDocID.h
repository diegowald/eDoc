#ifndef IDOCID_H
#define IDOCID_H

#include <QString>

struct IDocID
{
public:
    virtual QString asString() const = 0;
    virtual ~IDocID() {}
};

#endif // IDOCID_H
