#ifndef EDOCFACTORY_H
#define EDOCFACTORY_H

#include "edoc-factory_global.h"
#include "../eDoc-API/IDocEngine.h"
#include "../eDoc-API/IDatabase.h"
#include "../eDoc-API/IQueryEngine.h"
#include "../eDoc-API/ITagProcessor.h"
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
    virtual IQueryEngine *queryEngine();
    virtual ITagProcessor *tagEngine();
protected:
    void readAvailablePlugins();
    IDocEngine *createEngine();
    IDatabase *createDatabase();
    IQueryEngine *createQueryEngine();
    ITagProcessor *createTagEngine();

private:
    QString pluginPath;
    QString xmlFile;
    QMap<QString, QString> plugins;
    QMap<QString, QString> DBplugins;
    QMap<QString, QString> tagPlugins;
    IXMLContent *configuration;
    IDocEngine *engine;
    IDatabase *database;
    IQueryEngine *query;
    ITagProcessor *tagger;
    QObjectLogging *m_Logger;
};

#endif // EDOCFACTORY_H
