#ifndef IXMLCONTENT_H
#define IXMLCONTENT_H
#include <QSharedPointer>
#include <QString>
//#include <QsLog.h>
#include "edoc-configuration_global.h"

class EDOCCONFIGURATIONSHARED_EXPORT IXMLContent
{
public:
    virtual ~IXMLContent() {}
    virtual bool isLeaf() const = 0;
    virtual void key(const QString &value)
    {
        //QLOG_INFO() << "virtual void IXMLContent::key()";
        m_Key = value;
    }

    virtual QString key() const
    {
        //QLOG_INFO() << "virtual QString IXMLContent::key()";
        return m_Key;
    }

    virtual QString toDebugString()
    {
        return toDebugString(0);
    }

    virtual QString toDebugString(int indentation) = 0;

private:
    QString m_Key;
};

typedef QSharedPointer<IXMLContent> IXMLContentPtr;
#endif // IXMLCONTENT_H
