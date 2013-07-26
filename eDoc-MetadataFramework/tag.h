#ifndef TAG_H
#define TAG_H

#include <QMetaType>
#include <QObject>
#include <../eDoc-API/ITag.h>
#include "edoc-metadataframework_global.h"

class EDOCMETADATAFRAMEWORKSHARED_EXPORT Tag : public QObject, public ITag
{
    Q_OBJECT
public:
    explicit Tag(QObject *parent = 0);
    virtual ~Tag();
    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual QVariant content();
    virtual void setNull();
    virtual bool isNull();
    virtual QStringList keys();
    virtual QString asString();

signals:
    
public slots:
private:
    QString keywords;
    bool m_isNull;
};

Q_DECLARE_METATYPE(Tag*)
#endif // TAG_H
