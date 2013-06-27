#ifndef EDOCFACTORY_H
#define EDOCFACTORY_H

#include "edoc-factory_global.h"
#include "../eDoc-API/IDocEngine.h"
#include <QMap>


class EDOCFACTORYSHARED_EXPORT EDocFactory
{
public:
    EDocFactory();
    virtual ~EDocFactory();
    virtual void initialize(const QString &pluginPath, const QString &xmlFile);
    virtual IDocEngine* docEngine();

protected:
    void readAvailablePlugins();
private:
    QString pluginPath;
    QString xmlFile;
    QMap<QString, QString> plugins;
};

#endif // EDOCFACTORY_H
