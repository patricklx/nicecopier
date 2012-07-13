#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QObject>
#include <QLabel>
#include <QFontMetrics>

class QLabelEli : public QLabel
{
        Q_OBJECT
    public:
        explicit QLabelEli(QWidget *parent = 0):
            QLabel(parent){}

        void setText(QString text)
        {
            m_text = text;
            QFontMetrics metrics(this->font());
            QLabel::setToolTip(text);
            QString elidedtext = metrics.elidedText(text,Qt::ElideMiddle,this->width());
            QLabel::setText(elidedtext);
            setMinimumSize(0,0);
        }

    private:
        QString m_text;
        void resizeEvent(QResizeEvent *)
        {
            setText(m_text);
            setMinimumSize(0,0);
        }
};





#endif // MYQLABEL_H
