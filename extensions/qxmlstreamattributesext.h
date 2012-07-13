#ifndef QXMLSTREAMATTRIBUTESEXT_H
#define QXMLSTREAMATTRIBUTESEXT_H

#include <QXmlStreamWriter>
#include <QFile>
#include <QVariant>

class QXmlStreamWriterExt : public QXmlStreamWriter
{
    public:
        QXmlStreamWriterExt();
        QXmlStreamWriterExt(QFile* file):QXmlStreamWriter(file){}

        void writeAttribute(const QString &qualifiedName,QByteArray &array);

        void writeAttribute(const QString &qualifiedName, QVariant variant);
};

#endif // QXMLSTREAMATTRIBUTESEXT_H
