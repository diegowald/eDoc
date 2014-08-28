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
    virtual QSharedPointer<IDocEngine> docEngine();
    virtual QSharedPointer<IDatabaseWithHistory> databaseEngine();
    virtual QSharedPointer<IQueryEngine> queryEngine();
    virtual QSharedPointer<ITagProcessor> tagEngine();
    virtual QSharedPointer<IServer> serverEngine();
    virtual QSharedPointer<IRecord> createEmptyRecord();
    virtual void addDocument(const QString &filename, QSharedPointer<IRecord> record);

protected:
    virtual void addDocumentFromBlob(QByteArray &blob, const QString &filename, QSharedPointer<IRecord> record);
    void readAvailablePlugins();
    QSharedPointer<IDocEngine> createEngine();
    QSharedPointer<IDatabaseWithHistory> createDatabase();
    QSharedPointer<IDatabaseWithHistory> createDatabaseWithoutHistory();
    QSharedPointer<IQueryEngine> createQueryEngine();
    QSharedPointer<ITagProcessor> createTagEngine();
    QSharedPointer<IServer> createServerEngine();
private:
    QString pluginPath;
    QString xmlFile;
    QMap<QString, QString> plugins;
    QMap<QString, QString> DBWithHistoryPlugins;
    QMap<QString, QString> DBplugins;
    QMap<QString, QString> tagPlugins;
    QMap<QString, QString> serverPlugins;
    IXMLContent *configuration;
    QSharedPointer<IDocEngine> engine;
    QSharedPointer<IDatabaseWithHistory> database;
    QSharedPointer<IQueryEngine> query;
    QSharedPointer<ITagProcessor> tagger;
    QSharedPointer<QObjectLogging> m_Logger;
    QSharedPointer<IServer> server;
};

#endif // EDOCFACTORY_H
