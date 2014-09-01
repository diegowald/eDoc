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
    virtual QList<IFieldDefinitionPtr> fields() = 0;
    virtual IFieldDefinitionPtr field(const QString &fieldName) = 0;
    virtual IParameterPtr createEmptyParameter() = 0;
    virtual QList<IRecordIDPtr> search(const QList<IParameterPtr> &parameters) = 0;
    virtual QList<IRecordIDPtr> searchWithin(const QList<IParameterPtr> &parameters, const QList<IRecordIDPtr> &records) = 0;
    virtual IRecordPtr createEmptyRecord() = 0;
    virtual IRecordIDPtr addRecord(IRecordPtr record) = 0;
    virtual IRecordPtr getRecord(IRecordIDPtr id) = 0;
    virtual IRecordPtr getRecord(const QString &id) = 0;
    virtual QList<IRecordPtr> getRecords(const QStringList &ids) = 0;
    virtual void updateRecord(IRecordPtr record) = 0;
    virtual void deleteRecord(IRecordIDPtr id) = 0;
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName) = 0;
    virtual QList<QPair<QString, QString> > getColumnValue(const QList<QPair<QString, QString> >& filter, const QString & columnName) = 0;
    virtual QString name() = 0;

    virtual ~IDatabase() {}

protected:
    virtual QMap<QString, IRecordIDPtr > search(IParameterPtr parameter) = 0;
};

typedef QSharedPointer<IDatabase> IDatabasePtr;

Q_DECLARE_INTERFACE(IDatabase, "com.mksingenieria.eDoc.IDatabase/0.0")
#endif // IDATABASE_H
