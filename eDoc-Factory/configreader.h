#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QObject>
#include "IXMLContent.h"
#include <QXmlStreamReader>

class ConfigReader : public QObject
{
    Q_OBJECT
public:
    explicit ConfigReader(const QString &XMLFile, QObject *parent = 0);
    IXMLContent *getConfiguration();

private:
    void parseXML();
    IXMLContent *parseElements(QXmlStreamReader &xml);
signals:
    
public slots:
    

private:
    QString xmlFile;
    IXMLContent *xmlConfig;
};

#endif // CONFIGREADER_H
