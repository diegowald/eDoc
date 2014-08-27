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
    
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual QList<IFieldDefinition*> fields();
    virtual IFieldDefinition* field(const QString &fieldName);
    virtual QList<IRecordID*> search(const QList<IParameter*> &parameters);
    virtual QList<IRecordID*> searchWithin(const QList<IParameter*> &parameters, const QList<IRecordID*> &records);
    virtual IParameter* createEmptyParameter();
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

private:
    void createFields(IXMLContent* configuration);
    IFieldDefinition* createField(IXMLContent* configuration);
signals:
    
public slots:
    
private:
    QObjectLogging *m_Logger;
    QString m_Name;
    QMap<QString, IFieldDefinition*> m_Fields;
    QMap<QString, Record*> m_Records;
};

#endif // INMEMORYDATABASE_H
