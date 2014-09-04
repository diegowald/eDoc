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
    virtual long size();

signals:
    
public slots:
    
private:
    QSharedPointer<SimpleFileID> idDocument;
    QSharedPointer<FileManagement> fileManager;
    QString _filename;

};

#endif // SIMPLEFILEDOCUMENT_H
