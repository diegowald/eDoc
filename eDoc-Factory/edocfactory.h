#ifndef EDOCFACTORY_H
#define EDOCFACTORY_H

#include <memory>
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
    virtual void initialize(const QString &pluginPath, const QString &xmlFile, QSharedPointer<QObjectLogging> logger);
    virtual IDocEnginePtr docEngine();
    virtual IDatabaseWithHistoryPtr databaseEngine();
    virtual IQueryEnginePtr queryEngine();
    virtual ITagProcessorPtr tagEngine();
    virtual IServerPtr serverEngine();
    virtual IRecordPtr createEmptyRecord();
    virtual void addDocument(const QString &filename, QSharedPointer<IRecord> record);

protected:
    virtual void addDocumentFromBlob(QByteArray &blob, const QString &filename, QSharedPointer<IRecord> record);
    void readAvailablePlugins();
    IDocEnginePtr createEngine();
    IDatabaseWithHistoryPtr createDatabase();
    IDatabaseWithHistoryPtr createDatabaseWithoutHistory();
    QSharedPointer<IQueryEngine> createQueryEngine();
    ITagProcessorPtr createTagEngine();
    IServerPtr createServerEngine();
private:
    QString pluginPath;
    QString xmlFile;
    QMap<QString, QString> plugins;
    QMap<QString, QString> DBWithHistoryPlugins;
    QMap<QString, QString> DBplugins;
    QMap<QString, QString> tagPlugins;
    QMap<QString, QString> serverPlugins;
    IXMLContentPtr configuration;
    IDocEnginePtr engine;
    IDatabaseWithHistoryPtr database;
    IQueryEnginePtr query;
    ITagProcessorPtr tagger;
    QObjectLoggingPtr m_Logger;
    IServerPtr server;
};

#endif // EDOCFACTORY_H
