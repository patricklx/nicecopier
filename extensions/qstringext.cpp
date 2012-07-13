#include "qstringext.h"


QStringExt QStringExt::afterLast(QChar word)
{
    int index = lastIndexOf(word)+1;
    return right(size()-index);
}

QStringExt QStringExt::afterLast(QString word)
{
    int index = lastIndexOf(word)+1;
    return right(size()-index);
}

QStringExt QStringExt::beforeLast(QChar word)
{
    int index = lastIndexOf(word);
    return left(index);
}

QStringExt QStringExt::beforeLast(QString word)
{
    int index = lastIndexOf(word);
    return left(index);
}

QStringExt QStringExt::afterFirst(QString word)
{
    int index = indexOf(word)+1;
    return right(index);
}

QStringExt QStringExt::afterFirst(QChar word)
{
    int index = indexOf(word)+1;
    return right(index);
}

QStringExt QStringExt::beforeFirst(QString word)
{
    int index = indexOf(word);
    return left(index);
}

QStringExt QStringExt::beforeFirst(QChar word)
{
    int index = indexOf(word);
    return left(index);
}
