#include "messagecodes.h"

QString MessageCodes::toString(CodeNumber code)
{
    QString value = "";
    switch (code)
    {
    case CodeNumber::REQ_fields:
        value = "REQ:fields";
        break;
    case CodeNumber::REQ_field:
        value = "REQ:field";
        break;
    case CodeNumber::REQ_createEmptyParameter:
        value = "REQ:createEmptyParameter";
        break;
    case CodeNumber::REQ_search:
        value = "REQ:search";
        break;
    case CodeNumber::REQ_createEnptyRecord:
        value = "REQ:createEnptyRecord";
        break;
    case CodeNumber::REQ_addRecord:
        value = "REQ:addRecord";
        break;
    case CodeNumber::REQ_getRecord:
        value = "REQ:getRecord";
        break;
    case CodeNumber::REQ_updateRecord:
        value = "REQ:updateRecord";
        break;
    case CodeNumber::REQ_deleteRecord:
        value = "REQ:deleteRecord";
        break;
    case CodeNumber::REQ_getDistinctColumnValues:
        value = "REQ:getDistinctColumnValues";
        break;
    case CodeNumber::RSP_fields:
        value = "RSP:fields";
        break;
    case CodeNumber::RSP_field:
        value = "RSP:field";
        break;
    case CodeNumber::RSP_createEmptyParameter:
        value = "RSP:createEmptyParameter";
        break;
    case CodeNumber::RSP_search:
        value = "RSP:search";
        break;
    case CodeNumber::RSP_createEnptyRecord:
        value = "RSP:createEnptyRecord";
        break;
    case CodeNumber::RSP_addRecord:
        value = "RSP:addRecord";
        break;
    case CodeNumber::RSP_getRecord:
        value = "RSP:getRecord";
        break;
    case CodeNumber::RSP_updateRecord:
        value = "RSP:updateRecord";
        break;
    case CodeNumber::RSP_deleteRecord:
        value = "RSP:deleteRecord";
        break;
    case CodeNumber::RSP_getDistinctColumnValues:
        value = "RSP:getDistinctColumnValues";
        break;
    default:
        value = "ERROR";
    }
    return value;
}

MessageCodes::CodeNumber MessageCodes::fromString(const QString &codeString)
{
    CodeNumber retValue = CodeNumber::ERROR;
    if (codeString == "ERROR")
    {
        retValue = CodeNumber::ERROR;
    }
    else if (codeString == "REQ:fields")
    {
        retValue = CodeNumber::REQ_fields;
    }
    else if (codeString == "REQ:field")
    {
        retValue = CodeNumber::REQ_field;
    }
    else if (codeString == "REQ:createEmptyParameter")
    {
        retValue = CodeNumber::REQ_createEmptyParameter;
    }
    else if (codeString == "REQ:search")
    {
        retValue = CodeNumber::REQ_search;
    }
    else if (codeString == "REQ:createEnptyRecord")
    {
        retValue = CodeNumber::REQ_createEnptyRecord;
    }
    else if (codeString == "REQ:addRecord")
    {
        retValue = CodeNumber::REQ_addRecord;
    }
    else if (codeString == "REQ:getRecord")
    {
        retValue = CodeNumber::REQ_getRecord;
    }
    else if (codeString == "REQ:updateRecord")
    {
        retValue = CodeNumber::REQ_updateRecord;
    }
    else if (codeString == "REQ:deleteRecord")
    {
        retValue = CodeNumber::REQ_deleteRecord;
    }
    else if (codeString == "REQ:getDistinctColumnValues")
    {
        retValue = CodeNumber::REQ_getDistinctColumnValues;
    }
    else if (codeString == "RSP:fields")
    {
        retValue = CodeNumber::RSP_fields;
    }
    else if (codeString == "RSP:field")
    {
        retValue = CodeNumber::RSP_field;
    }
    else if (codeString == "RSP:createEmptyParameter")
    {
        retValue = CodeNumber::RSP_createEmptyParameter;
    }
    else if (codeString == "RSP:search")
    {
        retValue = CodeNumber::RSP_search;
    }
    else if (codeString == "RSP:createEnptyRecord")
    {
        retValue = CodeNumber::RSP_createEnptyRecord;
    }
    else if (codeString == "RSP:addRecord")
    {
        retValue = CodeNumber::RSP_addRecord;
    }
    else if (codeString == "RSP:getRecord")
    {
        retValue = CodeNumber::RSP_getRecord;
    }
    else if (codeString == "RSP:updateRecord")
    {
        retValue = CodeNumber::RSP_updateRecord;
    }
    else if (codeString == "RSP:deleteRecord")
    {
        retValue = CodeNumber::RSP_deleteRecord;
    }
    else if (codeString == "RSP:getDistinctColumnValues")
    {
        retValue = CodeNumber::RSP_getDistinctColumnValues;
    }

    return retValue;
}
