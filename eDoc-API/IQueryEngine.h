#ifndef IQUERYENGINE_H
#define IQUERYENGINE_H

#include <QStringList>
#include "../eDoc-Configuration/xmlcollection.h"

struct IQueryEngine
{
    virtual ~IQueryEngine() {}

    virtual void initialize(XMLCollection *configuration) = 0;
    virtual QStringList getTreeNames() const = 0;
    virtual QStringList getFieldsForTree(const QString &viewName) = 0;
};

#endif // IQUERYENGINE_H
