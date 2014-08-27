#ifndef EDOCFACTORY_H
#define EDOCFACTORY_H

#include "edoc-factory_global.h"
#include "../eDoc-API/IDocEngine.h"
#include "../eDoc-API/IDatabase.h"
#include "../eDoc-API/IDatabaseWithHistory.h"
#include "../eDoc-API/IQueryEngine.h"
#include "../eDoc-API/ITagProcessor.h"
#include "../eDoc-API/IServer.h"
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
    virtual IDatabaseWithHistory* databaseEngine();
    virtual IQueryEngine *queryEngine();
    virtual ITagProcessor *tagEngine();
    virtual IServer *serverEngine();
    virtual IRecord* createEmptyRecord();
    virtual void addDocument(const QString &filename, IRecord* record);

protected:
    virtual void addDocumentFromBlob(QByteArray &blob, const QString &filename, IRecord* record);
    void readAvailablePlugins();
    IDocEngine *createEngine();
    IDatabaseWithHistory *createDatabase();
    IDatabaseWithHistory *createDatabaseWithoutHistory();
    IQueryEngine *createQueryEngine();
    ITagProcessor *createTagEngine();
    IServer *createServerEngine();
private:
    QString pluginPath;
    QString xmlFile;
    QMap<QString, QString> plugins;
    QMap<QString, QString> DBWithHistoryPlugins;
    QMap<QString, QString> DBplugins;
    QMap<QString, QString> tagPlugins;
    QMap<QString, QString> serverPlugins;
    IXMLContent *configuration;
    IDocEngine *engine;
    IDatabaseWithHistory *database;
    IQueryEngine *query;
    ITagProcessor *tagger;
    QObjectLogging *m_Logger;
    IServer *server;
};

#endif // EDOCFACTORY_H
