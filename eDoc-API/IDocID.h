#ifndef IDOCID_H
#define IDOCID_H

#include <QMetaType>
#include <QString>
#include "forward.h"

struct IDocID
{
public:
    virtual QString asString() const = 0;
    virtual ~IDocID() {}
};


Q_DECLARE_METATYPE(IDocIDPtr)

#endif // IDOCID_H
