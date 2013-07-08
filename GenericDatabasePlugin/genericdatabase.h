#ifndef GENERICDATABASE_H
#define GENERICDATABASE_H

#include <QObject>
#include "../eDoc-API/IDatabase.h"
#include <QMap>
//#include "record.h"

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
    virtual QList<IRecordID*> search(const QList<IParameter*> &parameters) const;
    virtual IRecord* createEmptyRecord();
    virtual IRecordID *addRecord(IRecord *record);
    virtual IRecord* getRecord(IRecordID *id);
    virtual void updateRecord(IRecord* record);
    virtual void deleteRecord(IRecordID *id);
    virtual QString name();

    
signals:
    
public slots:
    
};

#endif // GENERICDATABASE_H
