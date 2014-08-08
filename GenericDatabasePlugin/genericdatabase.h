#ifndef GENERICDATABASE_H
#define GENERICDATABASE_H

#include <QObject>
#include "../eDoc-API/IDatabase.h"
#include <QMap>
#include "sqlmanager.h"
#include "../eDoc-API/ITagProcessor.h"
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

    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);
    virtual QList<IFieldDefinition*> fields();
    virtual IFieldDefinition* field(const QString &fieldName);
    virtual QList<IRecordID*> search(const QList<IParameter*> &parameters);
    virtual IParameter* createEmptyParameter();
    virtual IRecord* createEmptyRecord();
    virtual IRecordID *addRecord(IRecord *record);
    virtual IRecord* getRecord(IRecordID *id);
    virtual void updateRecord(IRecord* record);
    virtual void deleteRecord(IRecordID *id);
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QString name();

protected:
    virtual QMap<QString, IRecordID*> search(IParameter* parameter);
    virtual QMap<QString, IRecordID*> intersect(const QMap<QString, IRecordID*> &set1, const QMap<QString, IRecordID*> &set2);
private:
    void createFields(IXMLContent* configuration);
    IFieldDefinition *createField(IXMLContent *configuration);
    QString getFieldsString();
    QString getUpdateFieldsString();
    QString getParametersString();
    void executeSQLCommand(const QString &sql, IRecord* record);
    std::pair<QString, DBRecordPtr> getWhereClause(IParameter *parameter);

signals:
    
public slots:

private:
    QObjectLogging *m_Logger;
    QString m_Name;
    QMap<QString, IFieldDefinition*> m_Fields;
    QMap<QString, IFieldDefinition*> m_FieldsBasedOnDatabase;
    QString m_TableName;
    SQLManager m_SQLManager;
    ITagProcessor *m_TagProcessor;
};

#endif // GENERICDATABASE_H
