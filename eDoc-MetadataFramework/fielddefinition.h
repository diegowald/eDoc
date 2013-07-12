#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QObject>
#include "../eDoc-API/IFieldDefinition.h"
#include "../eDoc-Configuration/qobjectlgging.h"
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
    IMULTIDOCUMENT_TYPE // multidocument
};

class EDOCMETADATAFRAMEWORKSHARED_EXPORT FieldDefinition : public QObject, public IFieldDefinition
{
    Q_OBJECT
public:
    explicit FieldDefinition(QObject *parent = 0);
    virtual ~FieldDefinition();
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger);
    virtual QString name();
    virtual QString type();
    virtual bool isReadOnly();
    virtual bool isVisible();
    virtual QList<VALIDQUERY> validQueries();    
    virtual IValue* createEmptyValue();

    QString fieldNameInDatabase();
    DATATYPE dataType();
private:
    DATATYPE analyzeType();
signals:
    
public slots:
private:
    QString m_Name;
    QString m_Type;
    bool m_ReadOnly;
    bool m_Visible;
    QList<VALIDQUERY> m_ValidQeries;
    QObjectLogging *m_Logger;
    DATATYPE m_DataType;
    QString m_FieldNameInDatabase;
};

#endif // FIELDDEFINITION_H
