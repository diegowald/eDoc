#ifndef IQUERYENGINE_H
#define IQUERYENGINE_H

#include "forward.h"
#include <QStringList>
#include "IInitializable.h"

struct IQueryEngine : public IInitializable
{
    virtual ~IQueryEngine() {}

    virtual QStringList getTreeNames() const = 0;
    virtual QStringList getFieldsForTree(const QString &viewName) = 0;
};


#endif // IQUERYENGINE_H
