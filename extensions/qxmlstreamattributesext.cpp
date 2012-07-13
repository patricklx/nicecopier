#include "qxmlstreamattributesext.h"

QXmlStreamWriterExt::QXmlStreamWriterExt()
{
}

void QXmlStreamWriterExt::writeAttribute(const QString &qualifiedName, QByteArray &array)
{
    QXmlStreamWriter::writeAttribute(qualifiedName,QString(array));
}

void QXmlStreamWriterExt::writeAttribute(const QString &qualifiedName, QVariant variant)
{
    QXmlStreamWriter::writeAttribute(qualifiedName, variant.toString());
}
