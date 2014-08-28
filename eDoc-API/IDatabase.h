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
    virtual QList<QSharedPointer<IFieldDefinition> > fields() = 0;
    virtual QSharedPointer<IFieldDefinition> field(const QString &fieldName) = 0;
    virtual QSharedPointer<IParameter> createEmptyParameter() = 0;
    virtual QList<QSharedPointer<IRecordID> > search(const QList<QSharedPointer<IParameter> > &parameters) = 0;
    virtual QList<QSharedPointer<IRecordID> > searchWithin(const QList<QSharedPointer<IParameter> > &parameters, const QList<QSharedPointer<IRecordID> > &records) = 0;
    virtual QSharedPointer<IRecord> createEmptyRecord() = 0;
    virtual QSharedPointer<IRecordID> addRecord(QSharedPointer<IRecord> record) = 0;
    virtual QSharedPointer<IRecord> getRecord(QSharedPointer<IRecordID> id) = 0;
    virtual QSharedPointer<IRecord> getRecord(const QString &id) = 0;
    virtual QList<QSharedPointer<IRecord> > getRecords(const QStringList &ids) = 0;
    virtual void updateRecord(QSharedPointer<IRecord> record) = 0;
    virtual void deleteRecord(QSharedPointer<IRecordID> id) = 0;
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName) = 0;
    virtual QString name() = 0;

    virtual ~IDatabase() {}

protected:
    virtual QMap<QString, QSharedPointer<IRecordID> > search(QSharedPointer<IParameter> parameter) = 0;
};

Q_DECLARE_INTERFACE(IDatabase, "com.mksingenieria.eDoc.IDatabase/0.0")
#endif // IDATABASE_H
