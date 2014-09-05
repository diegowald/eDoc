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

    virtual void initialize(IXMLContentPtr configuration,
                            QObjectLoggingPtr logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual void addTagRecord(IRecordIDPtr recordID, ITagPtr tag);
    virtual void processKeywordString(IRecordIDPtr recordID, const QString &keywords);
    virtual void processKeywordString(IRecordIDPtr recordID, const QStringList &keywords);
    virtual QSet<QString> findByTags(const QStringList &tags);
    virtual void removeRecord(IRecordIDPtr recordID, ITagPtr tag);
    virtual QString name();
    virtual ITagProcessorPtr newTagProcessor();

private:
    void loadIntoMemory();
    void saveKeyword(IRecordIDPtr recordID, const QString &keyword, bool newKeyword);
    void bulkSave();

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

    struct BULKSAVERECORD
    {
        IRecordIDPtr record;
        QString tagString;
        bool newElement;
    };
    QList<BULKSAVERECORD> recordsToSave;
};

#endif // INMEMORYTAGPROCESSOR_H
