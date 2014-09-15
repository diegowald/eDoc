#include "filemanagement.h"
#include <QFile>
#include <QFileInfo>

FileManagement::FileManagement(const QString &folder, QObject *parent) :
    QObject(parent)
{
    m_Folder = folder;
}

void FileManagement::createFile(QString fileName, const QByteArray &blob)
{
    QString fn(m_Folder + "/" + fileName);
    QFile file(fn);
    file.open(QIODevice::WriteOnly);
    file.write(blob);
    file.close();
}

QByteArray FileManagement::readFile(QString fileName)
{
    QString fn(m_Folder + "/" + fileName);
    QFile file(fn);
    QByteArray ret;
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        ret = file.readAll();
        file.close();
    }
    return ret;
}

long FileManagement::size(QString filename)
{
    QFileInfo fileInfo(filename);
    return fileInfo.size();
}
