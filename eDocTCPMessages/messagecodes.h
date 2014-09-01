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
        REQ_searchWithin,
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
        RSP_searchWithin,
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
        // IDatabaseWithHistory
        REQ_searchWithHistory,
        REQ_getRecordWithHistory,
        REQ_getRecordsWithHistory,
        REQ_getDistinctColumnValuesWithHistory,
        REQ_getHistory,
        REQ_getHistoryChanges,
        RSP_searchWithHistory,
        RSP_getRecordWithHistory,
        RSP_getRecordsWithHistory,
        RSP_getDistinctColumnValuesWithHistory,
        RSP_getHistory,
        RSP_getHistoryChanges,
        // ITagProxessor
        REQ_addTagRecord,
        REQ_findByTags,
        REQ_removeRecord,
        REQ_processKeywordString,
        RSP_addTagRecord,
        RSP_findByTags,
        RSP_removeRecord,
        RSP_processKeywordString,
    };

    static QString toString(CodeNumber code);
    static CodeNumber fromString(const QString &codeString);
};

#endif // MESSAGECODES_H
