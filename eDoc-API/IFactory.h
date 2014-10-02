#ifndef IFACTORY_H
#define IFACTORY_H

#include <QSharedPointer>
#include "forward.h"
#include "IDocEngine.h"
#include "IDatabase.h"
#include "IQueryEngine.h"
#include "ITagProcessor.h"
#include "IServer.h"



class IFactory
{
public:
    virtual IDocEnginePtr createEngine(IXMLContentPtr configuration) = 0;
    virtual IDatabasePtr createDatabase(IXMLContentPtr configuration) = 0;
    virtual IDatabaseWithHistoryPtr createDatabaseWithHistory(IXMLContentPtr configuration) = 0;
    virtual IQueryEnginePtr createQueryEngine(IXMLContentPtr configuration) = 0;
    virtual ITagProcessorPtr createTagProcessor(IXMLContentPtr configuration) = 0;
    virtual IServerPtr createServer(IXMLContentPtr configuration) = 0;

    virtual QObjectLoggingPtr logger() = 0;

    virtual ~IFactory() {}
};

#endif // IFACTORY_H
