#ifndef SIMPLEFILEDOCUMENT_H
#define SIMPLEFILEDOCUMENT_H

#include <QObject>
#include "../eDoc-API/IDocument.h"
#include "simplefileid.h"
#include "filemanagement.h"

class SimpleFileDocument : public QObject, public IDocument
{
    Q_OBJECT
public:
    explicit SimpleFileDocument(FileManagement *FileManager, QObject *parent = 0);
    SimpleFileDocument(FileManagement *FileManager, QString stringID, QObject *parent = 0);
    virtual ~SimpleFileDocument();
    virtual IDocID* id();
    virtual QByteArray blob();

signals:
    
public slots:
    
private:
    SimpleFileID *idDocument;
    FileManagement *fileManager;
};

#endif // SIMPLEFILEDOCUMENT_H
