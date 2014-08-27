#ifndef IDATABASEWITHHISTORY_H
#define IDATABASEWITHHISTORY_H

#include "IDatabase.h"

struct IDatabaseWithHistory : public IDatabase
{
public:
    virtual QList<IRecordID*> searchByDate(const QList<IParameter*> &parameters, const QDateTime &date) = 0;
    virtual IRecord* getRecordByDate(IRecordID *id, const QDateTime &date) = 0;
    virtual IRecord* getRecordByDate(const QString &id, const QDateTime &date) = 0;
    virtual QList<IRecord*> getRecordsByDate(const QStringList &ids, const QDateTime& date) = 0;
    virtual QStringList getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date) = 0;
    virtual QList<IRecord*> getHistory(IRecordID *recordID) = 0;
    virtual QList<IRecordID*> getChanges(const QDateTime &fromDate, const QDateTime &toDate) = 0;

    virtual ~IDatabaseWithHistory() {}
protected:
    virtual QMap<QString, IRecordID*> searchByDate(IParameter* parameter, const QDateTime &date) = 0;
};

Q_DECLARE_INTERFACE(IDatabaseWithHistory, "com.mksingenieria.eDoc.IDatabaseWithHistory/0.0")
#endif // IDATABASEWITHHISTORY_H
