#ifndef MESSAGECODES_H
#define MESSAGECODES_H

#include <QString>
#include "edoctcpmessages_global.h"

class EDOCTCPMESSAGESSHARED_EXPORT MessageCodes
{
public:

    enum class CodeNumber
    {
        ERROR,
        // IDatabase
        REQ_fields,
        REQ_field,
        REQ_createEmptyParameter,
        REQ_search,
        REQ_createEnptyRecord,
        REQ_addRecord,
        REQ_getRecord,
        REQ_getRecords,
        REQ_updateRecord,
        REQ_deleteRecord,
        REQ_getDistinctColumnValues,
        RSP_fields,
        RSP_field,
        RSP_createEmptyParameter,
        RSP_search,
        RSP_createEnptyRecord,
        RSP_addRecord,
        RSP_getRecord,
        RSP_getRecords,
        RSP_updateRecord,
        RSP_deleteRecord,
        RSP_getDistinctColumnValues,
        // IDocEngine
        REQ_addDocument,
        REQ_getDocument,
        REQ_deleteDocument,
        REQ_IValueToIDocId,
        RSP_addDocument,
        RSP_getDocument,
        RSP_deleteDocument,
        RSP_IValueToIDocId,
    };

    static QString toString(CodeNumber code);
    static CodeNumber fromString(const QString &codeString);
};

#endif // MESSAGECODES_H
