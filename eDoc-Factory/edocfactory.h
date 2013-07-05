#ifndef EDOCFACTORY_H
#define EDOCFACTORY_H

#include "edoc-factory_global.h"
#include "../eDoc-API/IDocEngine.h"
#include "../eDoc-API/IDatabase.h"
#include <QMap>
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"

class EDOCFACTORYSHARED_EXPORT EDocFactory
{
public:
    EDocFactory();
    virtual ~EDocFactory();
    virtual void initialize(const QString &pluginPath, const QString &xmlFile, QObjectLogging *logger);
    virtual IDocEngine* docEngine();
    virtual IDatabase* databaseEngine();

protected:
    void readAvailablePlugins();
    IDocEngine *createEngine();
    IDatabase *createDatabase();
private:
    QString pluginPath;
    QString xmlFile;
    QMap<QString, QString> plugins;
    QMap<QString, QString> DBplugins;
    IXMLContent *configuration;
    IDocEngine *engine;
    IDatabase *database;
    QObjectLogging *m_Logger;
};

#endif // EDOCFACTORY_H
