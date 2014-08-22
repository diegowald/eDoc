#ifndef EDOCTCPCLIENT_H
#define EDOCTCPCLIENT_H

#include "edoctcpclient_global.h"
#include <QObject>
#include <QtNetwork/QTcpSocket>

//#include <QtNetwork/QNetworkSession>

#include "../eDoc-API/IDatabase.h"
#include "../eDoc-API/IDocEngine.h"
#include "../eDocTCPMessages/header.h"
#include "../eDocTCPMessages/messagecodes.h"

class EDOCTCPCLIENTSHARED_EXPORT eDocTcpClientDatabasePlugin : public QObject, public IDatabase, public IDocEngine
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDatabase" FILE "eDocTcpClient.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDatabase)
    Q_INTERFACES(IDocEngine)

public:
    explicit eDocTcpClientDatabasePlugin(QObject *parent = 0);
    ~eDocTcpClientDatabasePlugin();

    // IDatabase
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual QList<IFieldDefinition*> fields();
    virtual IFieldDefinition* field(const QString &fieldName);
    virtual IParameter* createEmptyParameter();
    virtual QList<IRecordID*> search(const QList<IParameter*> &parameters);
    virtual IRecord* createEmptyRecord();
    virtual IRecordID *addRecord(IRecord *record);
    virtual IRecord* getRecord(IRecordID *id);
    virtual IRecord* getRecord(const QString &id);
    virtual QList<IRecord*> getRecords(const QStringList &ids);
    virtual void updateRecord(IRecord* record);
    virtual void deleteRecord(IRecordID *id);
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QString name();

protected:
    virtual QMap<QString, IRecordID*> search(IParameter* parameter);

public:
    // IDocEngine
    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocBase* getDocument(IDocID *id);
    virtual bool deleteDocument(IDocID *id);
    virtual IDocID* IValueToIDocId(IValue* value);


private:
    void process(Header &header);

    void prepareToSend(MessageCodes::CodeNumber code);
    QByteArray send();

private slots:
    void readyRead();
    void error(QAbstractSocket::SocketError socketError);
    void connected();

private:
    QObjectLogging *logger;
    QTcpSocket *tcpSocket;
    //QNetworkSession *networkSession;
    QString ipAddress;
    int port;
    QString m_Name;
    int blockSize;
    int timeOut;

    QByteArray blob;

    QByteArray buildingBlob;
    QDataStream *out;

};

#endif // EDOCTCPCLIENT_H

