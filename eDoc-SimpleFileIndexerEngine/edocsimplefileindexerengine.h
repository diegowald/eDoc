#ifndef EDOCSIMPLEFILEINDEXERENGINE_H
#define EDOCSIMPLEFILEINDEXERENGINE_H

#include "edoc-simplefileindexerengine_global.h"
#include "../eDoc-API/IDocEngine.h"
#include "../sqlmanager/sqlmanager.h"

class EDOCSIMPLEFILEINDEXERENGINESHARED_EXPORT EDocSimpleFileIndexerEngine : public QObject, public IDocEngine
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDocEngine/0.0" FILE "SimpleFileIndexerEngine.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDocEngine)

public:
    EDocSimpleFileIndexerEngine(QObject *parent = 0);
    virtual ~EDocSimpleFileIndexerEngine();

    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);
    virtual IDocBasePtr createDocument(const QByteArray& blob);
    virtual IDocBasePtr createDocument(const QString sourcePath, const QByteArray &blob);
    virtual IDocBasePtr getDocument(IDocIDPtr id);
    virtual IDocBasePtr getDocument(const QString &id);
    virtual bool deleteDocument(IDocIDPtr id);
    virtual IDocIDPtr IValueToIDocId(IValuePtr value);
    virtual QString name();

    virtual IDocEnginePtr newDocEngine();

private:
    QObjectLoggingPtr logger;
    SQLManager m_SQLManager;
    QString tableName;
};

#endif // EDOCSIMPLEFILEINDEXERENGINE_H
