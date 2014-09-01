#ifndef GENERICDATABASE_H
#define GENERICDATABASE_H

#include <QObject>
#include "../eDoc-API/IDatabase.h"
#include <QMap>
#include "sqlmanager.h"
#include <utility>

class GenericDatabase : public QObject, public IDatabase
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDatabase/0.0" FILE "GenericDatabasePlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDatabase)

public:
    explicit GenericDatabase(QObject *parent = 0);
    virtual ~GenericDatabase();

    virtual void initialize(QSharedPointer<IXMLContent> configuration,
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
    virtual QMap<QString, QSharedPointer<IRecordID>> intersect(const QMap<QString, QSharedPointer<IRecordID>> &set1, const QMap<QString, QSharedPointer<IRecordID>> &set2);
private:
    void createFields(QSharedPointer<IXMLContent> configuration);
    QSharedPointer<IFieldDefinition> createField(QSharedPointer<IXMLContent> configuration);
    QString getFieldsString();
    QString getUpdateFieldsString();
    QString getParametersString();
    void executeSQLCommand(const QString &sql, QSharedPointer<IRecord> record);
    std::pair<QString, DBRecordPtr> getWhereClause(QSharedPointer<IParameter> parameter);

signals:
    
public slots:

private:
    QSharedPointer<QObjectLogging> m_Logger;
    QString m_Name;
    QMap<QString, QSharedPointer<IFieldDefinition>> m_Fields;
    QMap<QString, QSharedPointer<IFieldDefinition>> m_FieldsBasedOnDatabase;
    QString m_TableName;
    SQLManager m_SQLManager;
};

#endif // GENERICDATABASE_H
