#ifndef IDATABASE_H
#define IDATABASE_H

#include <QtPlugin>
#include "IInitializable.h"
#include "IFieldDefinition.h"
#include "IRecordID.h"
#include "IRecord.h"
#include "APIEnums.h"
#include "IParameter.h"
#include <QMap>


struct IDatabase : public IInitializable {
public:
    virtual QList<IFieldDefinition*> fields() = 0;
    virtual IFieldDefinition* field(const QString &fieldName) = 0;
    virtual IParameter* createEmptyParameter() = 0;
    virtual QList<IRecordID*> search(const QList<IParameter*> &parameters) = 0;
    virtual IRecord* createEmptyRecord() = 0;
    virtual IRecordID *addRecord(IRecord *record) = 0;
    virtual IRecord* getRecord(IRecordID *id) = 0;
    virtual IRecord* getRecord(const QString &id) = 0;
    virtual void updateRecord(IRecord* record) = 0;
    virtual void deleteRecord(IRecordID *id) = 0;
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName) = 0;
    virtual QString name() = 0;

    virtual ~IDatabase() {}

protected:
    virtual QMap<QString, IRecordID*> search(IParameter* parameter) = 0;
};

Q_DECLARE_INTERFACE(IDatabase, "com.mksingenieria.eDoc.IDatabase/0.0")
#endif // IDATABASE_H
