#include "tcpAddDocumentRequest.h"

TCPAddDocumentRequest::TCPAddDocumentRequest(QObject *parent) :
    MessageBase(ADD_DOCUMENT, parent)
{
}

TCPAddDocumentRequest::~TCPAddDocumentRequest()
{
}

void TCPAddDocumentRequest::setBlob(const QByteArray &blob)
{
    m_Blob = blob;
}

QByteArray TCPAddDocumentRequest::asBlob()
{
    writeHeader();
    (*ds) << m_Blob;
    return block;
}
