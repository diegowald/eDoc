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

class EDOCTCPCLIENTSHARED_EXPORT eDocTcpClientDatabasePlugin : virtual public TcpClient, public IDatabase, public IDocEngine, public ITagProcessor
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
    virtual QList<IFieldDefinitionPtr> fields();
    virtual IFieldDefinitionPtr field(const QString &fieldName);
    virtual IParameterPtr createEmptyParameter();
    virtual QList<IRecordIDPtr> search(const QList<IParameterPtr> &parameters);
    virtual QList<IRecordIDPtr> searchWithin(const QList<IParameterPtr> &parameters, const QList<IRecordIDPtr> &records);
    virtual IRecordPtr createEmptyRecord();
    virtual IRecordIDPtr addRecord(IRecordPtr record);
    virtual IRecordPtr getRecord(IRecordIDPtr id);
    virtual IRecordPtr getRecord(const QString &id);
    virtual QList<IRecordPtr> getRecords(const QStringList &ids);
    virtual void updateRecord(IRecordPtr record);
    virtual void deleteRecord(IRecordIDPtr id);
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QList<QPair<QString, QString>> getColumnValue(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QString name();
    virtual IDatabasePtr newDatabase();
    virtual IDocEnginePtr newDocEngine();
    virtual ITagProcessorPtr newTagProcessor();

protected:
    virtual QMap<QString, IRecordIDPtr> search(IParameterPtr parameter);

public:
    // IDocEngine
    virtual IDocBasePtr createDocument(const QByteArray& blob);
    virtual IDocBasePtr createDocument(const QString sourcePath, const QByteArray &blob);
    virtual IDocBasePtr getDocument(IDocIDPtr id);
    virtual IDocBasePtr getDocument(const QString &id);
    virtual bool deleteDocument(IDocIDPtr id);
    virtual IDocIDPtr IValueToIDocId(IValuePtr value);

public:
    // ITagProcessor
    virtual void addTagRecord(IRecordIDPtr recordID, ITagPtr tag);
    virtual QSet<QString> findByTags(const QStringList &tags);
    virtual void removeRecord(IRecordIDPtr recordID, ITagPtr tag);
    virtual void processKeywordString(IRecordIDPtr recordID, const QString &keywords);
    virtual void processKeywordStringList(IRecordIDPtr recordID, const QStringList &keywords);
    virtual void processRecord(IRecordPtr record);
private slots:

private:
    QObjectLoggingPtr logger;
    QString m_Name;
};

#endif // EDOCTCPCLIENT_H

