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

    virtual void initialize(IXMLContent *configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual void addTagRecord(QSharedPointer<IRecordID> recordID, QSharedPointer<ITag> tag);
    virtual void processKeywordString(QSharedPointer<IRecordID> recordID, const QString &keywords);
    virtual void processKeywordString(QSharedPointer<IRecordID> recordID, const QStringList &keywords);
    virtual QSet<QString> findByTags(const QStringList &tags);
    virtual void removeRecord(QSharedPointer<IRecordID> recordID, QSharedPointer<ITag> tag);
    virtual QString name();

private:
    void loadIntoMemory();
    void saveKeyword(QSharedPointer<IRecordID> recordID, const QString &keyword);

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
};

#endif // INMEMORYTAGPROCESSOR_H
