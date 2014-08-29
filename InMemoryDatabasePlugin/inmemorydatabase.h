#ifndef INMEMORYDATABASE_H
#define INMEMORYDATABASE_H

#include <QObject>
#include "../eDoc-API/IDatabase.h"
#include <QMap>
#include <../eDoc-MetadataFramework/record.h>

class InMemoryDatabase : public QObject, public IDatabase
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDatabase/0.0" FILE "InMemoryDatabasePlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDatabase)

public:
    explicit InMemoryDatabase(QObject *parent = 0);
    virtual ~InMemoryDatabase();
    
    virtual void initialize(IXMLContent *configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual QList<QSharedPointer<IFieldDefinition>> fields();
    virtual QSharedPointer<IFieldDefinition> field(const QString &fieldName);
    virtual QList<QSharedPointer<IRecordID>> search(const QList<QSharedPointer<IParameter>> &parameters);
    virtual QList<QSharedPointer<IRecordID>> searchWithin(const QList<QSharedPointer<IParameter>> &parameters, const QList<QSharedPointer<IRecordID>> &records);
    virtual QSharedPointer<IParameter> createEmptyParameter();
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

protected:
    virtual QMap<QString, QSharedPointer<IRecordID>> search(QSharedPointer<IParameter> parameter);

private:
    void createFields(IXMLContent* configuration);
    QSharedPointer<IFieldDefinition> createField(IXMLContent* configuration);
signals:
    
public slots:
    
private:
    QSharedPointer<QObjectLogging> m_Logger;
    QString m_Name;
    QMap<QString, QSharedPointer<IFieldDefinition>> m_Fields;
    QMap<QString, QSharedPointer<IRecord>> m_Records;
};

#endif // INMEMORYDATABASE_H
