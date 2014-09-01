#ifndef IQUERYENGINE_H
#define IQUERYENGINE_H

#include <QStringList>
#include "IInitializable.h"

struct IQueryEngine : public IInitializable
{
    virtual ~IQueryEngine() {}

    virtual QStringList getTreeNames() const = 0;
    virtual QStringList getFieldsForTree(const QString &viewName) = 0;
};

typedef QSharedPointer<IQueryEngine> IQueryEnginePtr;
#endif // IQUERYENGINE_H
