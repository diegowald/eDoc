#ifndef PROXYFIELDDEFINITION_H
#define PROXYFIELDDEFINITION_H

#include <QObject>
#include "edoctcpmessages_global.h"
#include "../eDoc-API/IFieldDefinition.h"

class EDOCTCPMESSAGESSHARED_EXPORT ProxyFieldDefinition : public QObject, public IFieldDefinition
{
    Q_OBJECT
public:
    explicit ProxyFieldDefinition(QObject *parent = 0);
    virtual ~ProxyFieldDefinition();

    virtual void initialize(QSharedPointer<IXMLContent> configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual QString name();
    virtual QString type();
    virtual bool isReadOnly() const;
    virtual bool isVisible() const;
    virtual bool isQueryable() const;
    virtual QList<VALIDQUERY> validQueries();
    virtual QSharedPointer<IValue> createEmptyValue();

    virtual void setName(const QString &newName);
    virtual void setType(const QString &newType);
    virtual void setIsReadOnly(bool value);
    virtual void setIsVisible(bool value);
    virtual void setIsQueryable(bool value);
    virtual void addValidQuery(VALIDQUERY query);

signals:

public slots:

private:
    QString _name;
    QString _type;
    bool _isReadOnly;
    bool _isVisible;
    bool _isQueryable;
    QList<VALIDQUERY> _validQueries;

};

#endif // PROXYFIELDDEFINITION_H
