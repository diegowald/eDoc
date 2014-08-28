#ifndef SIMPLEFILEDOCUMENT_H
#define SIMPLEFILEDOCUMENT_H

#include <memory>
#include <QObject>
#include "../eDoc-API/IDocument.h"
#include "simplefileid.h"
#include "filemanagement.h"

class SimpleFileDocument : public QObject, public IDocument
{
    Q_OBJECT
public:
    explicit SimpleFileDocument(QSharedPointer<FileManagement> FileManager, QObject *parent = 0);
    SimpleFileDocument(QSharedPointer<FileManagement> FileManager, QString stringID, QObject *parent = 0);
    virtual ~SimpleFileDocument();
    virtual QSharedPointer<IDocID> id();
    virtual QByteArray blob();

signals:
    
public slots:
    
private:
    QSharedPointer<SimpleFileID> idDocument;
    QSharedPointer<FileManagement> fileManager;
};

#endif // SIMPLEFILEDOCUMENT_H
