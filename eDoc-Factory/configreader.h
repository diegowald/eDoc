#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QObject>
#include "IXMLContent.h"
#include <QXmlStreamReader>
#include <QSharedPointer>

class ConfigReader : public QObject
{
    Q_OBJECT
public:
    explicit ConfigReader(const QString &XMLFile, QObject *parent = 0);
    QSharedPointer<IXMLContent> getConfiguration();

private:
    void parseXML();
    QSharedPointer<IXMLContent> parseElements(QXmlStreamReader &xml);
signals:
    
public slots:
    

private:
    QString xmlFile;
    QSharedPointer<IXMLContent> xmlConfig;
};

#endif // CONFIGREADER_H
