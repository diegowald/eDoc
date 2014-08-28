#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include "edoc-metadataframework_global.h"
#include "../eDoc-API/IRecord.h"
#include <QMap>
#include "../eDoc-API/IFieldDefinition.h"

class EDOCMETADATAFRAMEWORKSHARED_EXPORT Record : public QObject, public IRecord
{
    Q_OBJECT
public:
    explicit Record(const QList<QSharedPointer<IFieldDefinition>> &fieldDefs, QObject *parent = 0);
    virtual ~Record();

    virtual void setID(QSharedPointer<IRecordID> ID);
    virtual QSharedPointer<IRecordID> ID();
    virtual QSharedPointer<IValue> value(QSharedPointer<IFieldDefinition> field);
    virtual QSharedPointer<IValue> value(const QString &fieldName);
    virtual QSharedPointer<IFieldDefinition> fieldDefinition(const QString &fieldName);
    virtual QList<QString> fieldNames();
signals:
    
public slots:

private:
    QSharedPointer<IRecordID> m_ID;
    QMap<QString, QSharedPointer<IValue>> m_Values;
    QMap<QString, QSharedPointer<IFieldDefinition>> m_Fields;
};

#endif // RECORD_H
