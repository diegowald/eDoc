#ifndef EDOCTCPSERVERDATABASEPLUGIN_H
#define EDOCTCPSERVERDATABASEPLUGIN_H

#include "edoctcpserverplugin_global.h"
#include <QThread>
#include <QtNetwork/QTcpServer>

#include "../eDoc-API/IDatabase.h"
#include "../eDoc-API/IDatabaseWithHistory.h"
#include "../eDoc-API/IDocEngine.h"
#include "../eDoc-API/ITagProcessor.h"

#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDocTCPMessages/header.h"
#include "../eDocTCPMessages/messagecodes.h"



class EDOCTCPSERVERPLUGIN_EXPORT EDocTCPServerDatabasePlugin : public QThread
{
    Q_OBJECT
    typedef void (EDocTCPServerDatabasePlugin::*Executor) (QDataStream&);

public:
    EDocTCPServerDatabasePlugin(QObjectLoggingPtr Logger,
                                QSharedPointer<QTcpSocket> socket,
                                IDatabasePtr persistance,
                                IDatabaseWithHistoryPtr histPersistance,
                                IDocEnginePtr docEngine,
                                ITagProcessorPtr tagProcessor,
                                QObject *parent = 0);
    virtual ~EDocTCPServerDatabasePlugin();

    void run();

private slots:
    void onReadyRead();
    void error(QAbstractSocket::SocketError);
    void connected();

private:
    void process(QDataStream &in);

    void prepareToSend(MessageCodes::CodeNumber code);
    void send();

    void processREQFields(QDataStream &in);
    void processREQField(QDataStream &in);
    void processREQcreateEmptyParameter(QDataStream &in);
    void processREQSearch(QDataStream &in);
    void processREQSearchWithin(QDataStream &in);
    void processREQCreateEnptyRecord(QDataStream &in);
    void processREQAddRecord(QDataStream &in);
    void processREQGetRecord(QDataStream &in);
    void processREQGetRecords(QDataStream &in);
    void processREQUpdateRecord(QDataStream &in);
    void processREQDeleteRecord(QDataStream &in);
    void processREQGetDistinctColumnValues(QDataStream &in);
    void processREQAddDocument(QDataStream &in);
    void processREQSearchWithHistory(QDataStream &in);
    void processREQGetRecordWithHistory(QDataStream &in);
    void processREQGetRecordsWithHistory(QDataStream &in);
    void processREQGetDistinctColumnValuesWithHistory(QDataStream &in);
    void processREQGetHistory(QDataStream &in);
    void processREQGetHistoryChanges(QDataStream &in);
    void processREQAddTagRecord(QDataStream &in);
    void processREQFindByTags(QDataStream &in);
    void processREQRemoveRecord(QDataStream &in);
    void processREQprocessKeywordString(QDataStream &in);

private:
    QSharedPointer<QObjectLogging> logger;
    QString m_Name;
    QSharedPointer<QTcpSocket> _socket;
    IDatabasePtr _persistance;
    IDatabaseWithHistoryPtr _persistanceHist;
    IDocEnginePtr _docEngine;
    ITagProcessorPtr _tagProcessor;
    int blockSize;

    QByteArray blob;

    QByteArray buildingBlob;
    QDataStream *out;
    QMap<MessageCodes::CodeNumber, Executor> functionMap;
};

#endif // EDOCTCPSERVERDATABASEPLUGIN_H
