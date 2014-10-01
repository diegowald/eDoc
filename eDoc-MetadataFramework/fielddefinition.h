#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QObject>
#include "../eDoc-API/IFieldDefinition.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/IDocEngine.h"
#include "edoc-metadataframework_global.h"

enum DATATYPE {
    INVALID_TYPE, // any value not described below
    INTEGER_TYPE, // integer
    DOUBLE_TYPE,  // double
    BOOL_TYPE,    // boolean
    QSTRING_TYPE, // string
    QDATETIME_TYPE, // datetime
    QDATE_TYPE,     // date
    QTIME_TYPE,     // time
    IDOCBASE_TYPE,  // docbase
    IDOCUMENT_TYPE, // document
    IMULTIDOCUMENT_TYPE, // multidocument
    IRECORD_REFERENCE_TYPE, // references to other record in other database
    IMULTIRECORD_REFERENCE_TYPE, // references to an ordered set of records in other database
    TAG_TYPE // references to a list of keywords.
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT FieldDefinition : public QObject, public IFieldDefinition
{
    Q_OBJECT
public:
    explicit FieldDefinition(QObject *parent = 0);
    virtual ~FieldDefinition();
    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);
    virtual QString name();
    virtual QString type();
    virtual bool isReadOnly() const;
    virtual bool isVisible() const;
    virtual bool isQueryable() const;
    virtual QList<VALIDQUERY> validQueries();    
    virtual IValuePtr createEmptyValue();

    QString fieldNameInDatabase();
    DATATYPE dataType();
    IDocEnginePtr getEngine();
private:
    DATATYPE analyzeType();

signals:
    
public slots:
private:
    QString m_Name;
    QString m_Type;
    bool m_ReadOnly;
    bool m_Visible;
    bool m_Queryable;
    QList<VALIDQUERY> m_ValidQeries;
    QObjectLoggingPtr m_Logger;
    DATATYPE m_DataType;
    QString m_FieldNameInDatabase;
    QString m_OtherDatabaseName;
    QString m_FieldToShow;
    IDocEnginePtr engine;
};

#endif // FIELDDEFINITION_H
