#ifndef EDOCJSONSERVERPLUGIN_H
#define EDOCJSONSERVERPLUGIN_H

#include "edocjsonserverplugin_global.h"
#include <QThread>
#include <QtNetwork/QTcpServer>

#include "../eDoc-API/IDatabase.h"
#include "../eDoc-API/IDatabaseWithHistory.h"
#include "../eDoc-API/IDocEngine.h"
#include "../eDoc-API/ITagProcessor.h"

#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDocTCPMessages/header.h"
#include "../eDocTCPMessages/messagecodes.h"

class EDOCJSONSERVERPLUGINSHARED_EXPORT EDocJsonServerPlugin : public QThread
{
    Q_OBJECT
    typedef QJsonObject (EDocJsonServerPlugin::*Executor) (QJsonObject &input);

public:
    EDocJsonServerPlugin(QObjectLoggingPtr logger,
                         QSharedPointer<QTcpSocket> socket,
                         IDatabasePtr persistance,
                         IDatabaseWithHistoryPtr histPersistance,
                         IDocEnginePtr docEngine,
                         ITagProcessorPtr tagProcessor,
                         QObject *parent = 0);
    virtual ~EDocJsonServerPlugin();

    void run();

private slots:
    void onReadyRead();
    void error(QAbstractSocket::SocketError);
    void connected();

private:
    void process(QDataStream &in);

    void prepareToSend(MessageCodes::CodeNumber code);
    void send();

    QJsonObject processREQFields(QJsonObject &input);
    QJsonObject processREQField(QJsonObject &input);
/*    QJsonObject processREQcreateEmptyParameter(QJsonObject &input);
    QJsonObject processREQSearch(QJsonObject &input);
    QJsonObject processREQSearchWithin(QJsonObject &input);
    QJsonObject processREQCreateEnptyRecord(QJsonObject &input);
    QJsonObject processREQAddRecord(QJsonObject &input);
    QJsonObject processREQGetRecord(QJsonObject &input);
    QJsonObject processREQGetRecords(QJsonObject &input);
    QJsonObject processREQUpdateRecord(QJsonObject &input);
    QJsonObject processREQDeleteRecord(QJsonObject &input);
    QJsonObject processREQGetDistinctColumnValues(QJsonObject &input);
    QJsonObject processREQcreateDocument(QJsonObject &input);
    QJsonObject processREQcreateDocumentWithName(QJsonObject &input);
    QJsonObject processREQSearchWithHistory(QJsonObject &input);
    QJsonObject processREQGetRecordWithHistory(QJsonObject &input);
    QJsonObject processREQGetRecordsWithHistory(QJsonObject &input);
    QJsonObject processREQGetDistinctColumnValuesWithHistory(QJsonObject &input);
    QJsonObject processREQGetHistory(QJsonObject &input);
    QJsonObject processREQGetHistoryChanges(QJsonObject &input);
    QJsonObject processREQAddTagRecord(QJsonObject &input);
    QJsonObject processREQFindByTags(QJsonObject &input);
    QJsonObject processREQRemoveRecord(QJsonObject &input);
    QJsonObject processREQprocessKeywordString(QJsonObject &input);
    QJsonObject processREQprocessKeywordStringList(QJsonObject &input);*/

private:
    QSharedPointer<QObjectLogging> logger;
    QString m_Name;
    QSharedPointer<QTcpSocket> _socket;
    IDatabasePtr _persistance;
    IDatabaseWithHistoryPtr _persistanceHist;
    IDocEnginePtr _docEngine;
    ITagProcessorPtr _tagProcessor;
    int blockSize;

    QJsonObject response;

    QMap<MessageCodes::CodeNumber, Executor> functionMap;
};

#endif // EDOCJSONSERVERPLUGIN_H
