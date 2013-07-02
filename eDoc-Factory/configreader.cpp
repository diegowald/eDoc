#include "configreader.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include "xmlcollection.h"
#include "xmlelement.h"

ConfigReader::ConfigReader(const QString &XMLFile, QObject *parent) :
    QObject(parent)
{
    xmlFile = XMLFile;
    xmlConfig = NULL;
    parseXML();
}

void ConfigReader::parseXML()
{
    /* We'll parse the example.xml */
    QFile* file = new QFile(xmlFile);

    /* If we can't open it, let's show an error message. */
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        /*QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              "Couldn't open example.xml",
                              QMessageBox::Ok);*/
        return;
    }
    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xml(file);
    /* We'll parse the XML until we reach end of it.*/
    while(!xml.atEnd() &&
          !xml.hasError()) {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            xmlConfig = parseElements(xml);
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        /*QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              xml.errorString(),
                              QMessageBox::Ok);*/
    }
    /* Removes any device() or data from the reader
       * and resets its internal state to the initial state. */
    xml.clear();
    //this->addPersonsToUI(persons);
}

IXMLContent *ConfigReader::parseElements(QXmlStreamReader &xml)
{
    XMLCollection *config = new XMLCollection();
    config->key(xml.name().toString());

    QXmlStreamAttributes attributes = xml.attributes();
    foreach (QXmlStreamAttribute attr, attributes) {
        config->addXML(new XMLElement(attr.name().toString(), attr.value().toString(), config));
    }

    /* Next element... */
    xml.readNext();

    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
    */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == config->key()))
    {
        if(xml.tokenType() == QXmlStreamReader::StartElement)
        {
            if (xml.attributes().count() == 0)
            {
                QString name = xml.name().toString();
                xml.readNext();
                config->addXML(new XMLElement(name, xml.text().toString(), config));
            }
            else
            {
                config->addXML(parseElements(xml));
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return config;
}

IXMLContent *ConfigReader::getConfiguration()
{
    return xmlConfig;
}
