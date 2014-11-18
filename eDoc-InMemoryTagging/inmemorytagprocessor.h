#ifndef INMEMORYTAGPROCESSOR_H
#define INMEMORYTAGPROCESSOR_H

#include "edoc-inmemorytagging_global.h"
#include <QObject>
#include <QMap>
#include <QSet>
#include "../eDoc-API/IRecordID.h"
#include "../eDoc-API/ITag.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/ITagProcessor.h"
#include "../sqlmanager/sqlmanager.h"
#include "../eDoc-MetadataFramework/valuedefinitions.h"

class EDOCINMEMORYTAGGINGSHARED_EXPORT InMemoryTagProcessor : public QObject, public ITagProcessor
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.ITagProcessor/0.0" FILE "TagProcessorPlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(ITagProcessor)

public:
    explicit InMemoryTagProcessor(QObject *parent = 0);
    virtual ~InMemoryTagProcessor();

    virtual void initialize(IXMLContentPtr configuration, IFactory *factory);
    virtual void addTagRecord(IRecordIDPtr recordID, ITagPtr tag);
    virtual void processKeywordString(IRecordIDPtr recordID, const QString &keywords);
    virtual void processKeywordStringList(IRecordIDPtr, const QStringList &keywords);
    virtual void processRecord(IRecordPtr record);
    virtual QSet<QString> findByTags(const QStringList &tags);
    virtual void removeRecord(IRecordIDPtr recordID, ITagPtr tag);
    virtual QString name();
    virtual ITagProcessorPtr newTagProcessor();

private:
    void loadIntoMemory();
    void saveKeyword(IRecordIDPtr recordID, const QString &keyword, bool newKeyword);
    void bulkSave();
    void processValue(IRecordPtr record, const QString &fieldName);
    void processDocument(IRecordIDPtr recordID, IDocumentValuePtr &value);
signals:
    
public slots:

private:
    struct TAGElement
    {
        int id;
        bool saved;
        QSet<QString> occurrences;
    };

    QMap<QString, TAGElement> m_Tag;

    QSharedPointer<QObjectLogging> m_Logger;

    QString m_Name;
    QString m_keywordsTableName;
    QString m_indexTableName;
    SQLManager m_SQLManager;
    int maxIdUsed;

    int minStringLength;


    struct BULKSAVERECORD
    {
        IRecordIDPtr record;
        QString tagString;
        bool newElement;
    };
    QList<BULKSAVERECORD> recordsToSave;
};

#endif // INMEMORYTAGPROCESSOR_H
