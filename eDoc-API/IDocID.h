#ifndef IDOCID_H
#define IDOCID_H

#include <QMetaType>
#include <QString>
#include <QSharedPointer>

struct IDocID
{
public:
    virtual QString asString() const = 0;
    virtual ~IDocID() {}
};

Q_DECLARE_METATYPE(QSharedPointer<IDocID>)

#endif // IDOCID_H
