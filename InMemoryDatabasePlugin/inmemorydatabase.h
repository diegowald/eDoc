#ifndef INMEMORYDATABASE_H
#define INMEMORYDATABASE_H

#include <QObject>
#include "../eDoc-API/IDatabase.h"

class InMemoryDatabase : public QObject, public IDatabase
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDatabase/0.0" FILE "InMemoryDatabasePlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDatabase)

public:
    explicit InMemoryDatabase(QObject *parent = 0);
    virtual ~InMemoryDatabase();
    
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);
    virtual QList<IFieldDefinition*> fields();
    virtual QList<IRecord*> search(const QList<IParameter*> &parameters) const;
    virtual QString name();

signals:
    
public slots:
    
private:
    QObjectLogging *m_Logger;
    QString m_Name;
    QMap<QString, IFieldDefinition*> m_Fields;
    QMap<int, QMap<QString, IValue*> > m_Records;
};

#endif // INMEMORYDATABASE_H
