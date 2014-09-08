#ifndef FORWARD_H
#define FORWARD_H

#include <QSharedPointer>

#define CONSTRUCT(X) \
    struct X; \
    typedef QSharedPointer<X> X## Ptr;

CONSTRUCT(IDatabase)
CONSTRUCT(IDatabaseWithHistory)
CONSTRUCT(IDocBase)
CONSTRUCT(IDocEngine)
CONSTRUCT(IDocID)
CONSTRUCT(IDocument)
CONSTRUCT(IFactory)
CONSTRUCT(IFieldDefinition)
CONSTRUCT(IGroup)
CONSTRUCT(IGroupID)
CONSTRUCT(IMetadata)
CONSTRUCT(IMultiDocument)
CONSTRUCT(IMultiRecord)
CONSTRUCT(IParameter)
CONSTRUCT(IQueryEngine)
CONSTRUCT(IRecord)
CONSTRUCT(IRecordID)
CONSTRUCT(IServer)
CONSTRUCT(ITag)
CONSTRUCT(ITagProcessor)
CONSTRUCT(IUser)
CONSTRUCT(IUserID)
CONSTRUCT(IValue)

#endif // FORWARD_H
