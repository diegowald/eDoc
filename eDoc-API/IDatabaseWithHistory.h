#ifndef IDATABASEWITHHISTORY_H
#define IDATABASEWITHHISTORY_H

#include "forward.h"
#include "IDatabase.h"


struct IDatabaseWithHistory : public IDatabase
{
public:
    virtual QList<IRecordIDPtr> searchByDate(const QList<IParameterPtr> &parameters, const QDateTime &date) = 0;
    virtual IRecordPtr getRecordByDate(IRecordIDPtr id, const QDateTime &date) = 0;
    virtual IRecordPtr getRecordByDate(const QString &id, const QDateTime &date) = 0;
    virtual QList<IRecordPtr> getRecordsByDate(const QStringList &ids, const QDateTime& date) = 0;
    virtual QStringList getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date) = 0;
    virtual QList<IRecordPtr> getHistory(IRecordIDPtr recordID) = 0;
    virtual QList<IRecordIDPtr> getChanges(const QDateTime &fromDate, const QDateTime &toDate) = 0;

    virtual IDatabaseWithHistoryPtr newDatabaseWithHistory() = 0;

    virtual ~IDatabaseWithHistory() {}

protected:
    virtual QMap<QString, IRecordIDPtr> searchByDate(IParameterPtr parameter, const QDateTime &date) = 0;
};



Q_DECLARE_INTERFACE(IDatabaseWithHistory, "com.mksingenieria.eDoc.IDatabaseWithHistory/0.0")
#endif // IDATABASEWITHHISTORY_H
