#ifndef IQUERYENGINE_H
#define IQUERYENGINE_H

#include "forward.h"
#include <QStringList>
#include "IInitializable.h"

class IQueryEngine : public IInitializable
{
public:
    virtual ~IQueryEngine() {}

    virtual QStringList getTreeNames() const = 0;
    virtual QStringList getFieldsForTree(const QString &viewName) = 0;
};


#endif // IQUERYENGINE_H
