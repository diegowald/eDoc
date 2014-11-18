#ifndef APIENUMS_H
#define APIENUMS_H

enum class VALIDQUERY
{
    EQUALS_TO,
    DISTINT_TO,
    LESS_THAN,
    LESS_THAN_OR_EQUALS_TO,
    GREATER_THAN,
    GREATER_THAN_OR_EQUALS_TO,
    BETWEEN,
    CONTAINS,
    STARTS_WITH,
    ENDS_WITH,
    IS_NULL,
    IS_NOT_NULL
};

enum class DATATYPE {
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

#endif // APIENUMS_H
