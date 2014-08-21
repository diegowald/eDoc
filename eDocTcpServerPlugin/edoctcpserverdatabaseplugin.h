#ifndef EDOCTCPSERVERDATABASEPLUGIN_H
#define EDOCTCPSERVERDATABASEPLUGIN_H

#include "edoctcpserverplugin_global.h"
#include <QObject>
#include <QtNetwork/QTcpServer>

#include "../eDoc-API/IDatabase.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDocTCPMessages/header.h"
#include "../eDocTCPMessages/messagecodes.h"

class EDOCTCPSERVERPLUGIN_EXPORT EDocTCPServerDatabasePlugin : public QObject, public IDatabase
{
    Q_OBJECT
    //Q_INTERFACES(IDatabase)

public:
    EDocTCPServerDatabasePlugin(QTcpSocket *socket, IDatabase *persistance, QObject *parent = 0);
    virtual ~EDocTCPServerDatabasePlugin();
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
    virtual void updateRecord(IRecord* record);
    virtual void deleteRecord(IRecordID *id);
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QString name();

protected:
    virtual QMap<QString, IRecordID*> search(IParameter* parameter);

private slots:
    void onReadyRead();
    void error(QAbstractSocket::SocketError);
    void connected();

private:
    IDatabase *createPersistentEngine(XMLCollection *confEngine,
                                      const QMap<QString, QString> &docpluginStock,
                                      const QMap<QString, QString> &DBplugins,
                                      const QMap<QString, QString> &tagPlugins,
                                      const QMap<QString, QString> &serverPlugins);

    void process(QDataStream &in);

    void prepareToSend(MessageCodes::CodeNumber code);
    void send();

private:
    QObjectLogging *logger;
    QString m_Name;
    QTcpSocket *_socket;
    IDatabase *_persistance;

    int blockSize;

    QByteArray blob;

    QByteArray buildingBlob;
    QDataStream *out;

};

#endif // EDOCTCPSERVERDATABASEPLUGIN_H
