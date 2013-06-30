#ifndef EDOCFACTORY_H
#define EDOCFACTORY_H

#include "edoc-factory_global.h"
#include "../eDoc-API/IDocEngine.h"
#include <QMap>
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"

class EDOCFACTORYSHARED_EXPORT EDocFactory
{
public:
    EDocFactory();
    virtual ~EDocFactory();
    virtual void initialize(const QString &pluginPath, const QString &xmlFile, QObjectLgging *logger);
    virtual IDocEngine* docEngine();

protected:
    void readAvailablePlugins();
    IDocEngine *createEngine();
private:
    QString pluginPath;
    QString xmlFile;
    QMap<QString, QString> plugins;
    IXMLContent *configuration;
    IDocEngine *engine;
    QObjectLgging *m_Logger;
};

#endif // EDOCFACTORY_H
