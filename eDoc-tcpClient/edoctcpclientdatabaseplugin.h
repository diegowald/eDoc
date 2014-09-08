#ifndef EDOCTCPCLIENT_H
#define EDOCTCPCLIENT_H

#include "edoctcpclient_global.h"
#include <QObject>
#include <QtNetwork/QTcpSocket>

//#include <QtNetwork/QNetworkSession>

#include "../eDoc-API/IDatabase.h"
#include "../eDoc-API/IDocEngine.h"
#include "../eDoc-API/ITagProcessor.h"
#include "../eDocTCPMessages/header.h"
#include "../eDocTCPMessages/messagecodes.h"
#include "../eDocTCPMessages/tcpclient.h"

class EDOCTCPCLIENTSHARED_EXPORT eDocTcpClientDatabasePlugin : public TcpClient, public IDatabase, public IDocEngine, public ITagProcessor
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDatabase" FILE "eDocTcpClient.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDatabase)
    Q_INTERFACES(IDocEngine)
    Q_INTERFACES(ITagProcessor)

public:
    explicit eDocTcpClientDatabasePlugin(QObject *parent = 0);
    ~eDocTcpClientDatabasePlugin();

    // IDatabase
    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);
    virtual QList<QSharedPointer<IFieldDefinition>> fields();
    virtual QSharedPointer<IFieldDefinition> field(const QString &fieldName);
    virtual QSharedPointer<IParameter> createEmptyParameter();
    virtual QList<QSharedPointer<IRecordID>> search(const QList<QSharedPointer<IParameter>> &parameters);
    virtual QList<QSharedPointer<IRecordID>> searchWithin(const QList<QSharedPointer<IParameter>> &parameters, const QList<QSharedPointer<IRecordID>> &records);
    virtual QSharedPointer<IRecord> createEmptyRecord();
    virtual QSharedPointer<IRecordID> addRecord(QSharedPointer<IRecord> record);
    virtual QSharedPointer<IRecord> getRecord(QSharedPointer<IRecordID> id);
    virtual QSharedPointer<IRecord> getRecord(const QString &id);
    virtual QList<QSharedPointer<IRecord>> getRecords(const QStringList &ids);
    virtual void updateRecord(QSharedPointer<IRecord> record);
    virtual void deleteRecord(QSharedPointer<IRecordID> id);
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QList<QPair<QString, QString>> getColumnValue(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QString name();
    virtual IDatabasePtr newDatabase();
    virtual IDocEnginePtr newDocEngine();
    virtual ITagProcessorPtr newTagProcessor();

protected:
    virtual QMap<QString, QSharedPointer<IRecordID>> search(QSharedPointer<IParameter> parameter);

public:
    // IDocEngine
    virtual QSharedPointer<IDocID> addDocument(const QByteArray& blob);
    virtual QSharedPointer<IDocBase> getDocument(QSharedPointer<IDocID> id);
    virtual bool deleteDocument(QSharedPointer<IDocID> id);
    virtual QSharedPointer<IDocID> IValueToIDocId(QSharedPointer<IValue> value);

public:
    // ITagProcessor
    virtual void addTagRecord(QSharedPointer<IRecordID> recordID, QSharedPointer<ITag> tag);
    virtual QSet<QString> findByTags(const QStringList &tags);
    virtual void removeRecord(QSharedPointer<IRecordID> recordID, QSharedPointer<ITag> tag);
    virtual void processKeywordString(QSharedPointer<IRecordID> recordID, const QString &keywords);
    virtual void processKeywordStringList(IRecordIDPtr recordID, const QStringList &keywords);

private slots:

private:
    QSharedPointer<QObjectLogging> logger;
    QString m_Name;
};

#endif // EDOCTCPCLIENT_H

