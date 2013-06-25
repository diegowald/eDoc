#ifndef EDOCFACTORY_H
#define EDOCFACTORY_H

#include "edoc-factory_global.h"
#include <eDoc-API/IDocEngine.h>

class EDOCFACTORYSHARED_EXPORT EDocFactory
{
public:
    EDocFactory();
    virtual ~EDocFactory();

    void initialize(const QString &pluginPath, const QString &xmlFile);

    IDocEngine* docEngine();
};

#endif // EDOCFACTORY_H
