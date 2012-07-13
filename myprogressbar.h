#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QObject>
#include <QWidget>
#include <QProgressBar>
#include <QLabel>

class myprogressbar: public QProgressBar
{
        Q_OBJECT


    public:
        myprogressbar(QWidget *parent)
            :QProgressBar(parent)
        {
            progressbarText = new QLabel(this);
            progressbarText->setText("0.0%");
            progressbarText->move(rect().width()/2-progressbarText->rect().width()/2,rect().height()/2-progressbarText->rect().height()/2);
            progressbarText->hide();
            connect(this,SIGNAL(valueChanged(int)),SLOT(updateText(int)));
        }

    private:
        QLabel *progressbarText;
        inline QColor getContrastingTextColor(const QColor & c)
        {
            int a = qMax(c.red(),c.blue());
            a = qMax(a,c.blue());
            return (a >= 128) ? Qt::black : Qt::white;
        }

        void paintEvent(QPaintEvent *e)
        {
            progressbarText->move(rect().width()/2-progressbarText->rect().width()/2,rect().height()/2-progressbarText->rect().height()/2);
            QProgressBar::paintEvent(e);
        }


    private slots:
        void updateText(int value)
        {
            if( (maximum()==0 && minimum()==0) || this->value()==0 || !this->isVisible())
            {
                progressbarText->hide();
                return;
            }
            else
                progressbarText->show();
            float perc = ((1.0f*value)/this->maximum())*100.0f;
            QString text = QString("%1%").arg(perc,0,'f',1);
            progressbarText->setText(text);
            progressbarText->adjustSize();
            progressbarText->move(rect().width()/2-progressbarText->rect().width()/2,rect().height()/2-progressbarText->rect().height()/2);
            if(perc>50)
            {
                QPalette palette = progressbarText->palette();
                QColor color = getContrastingTextColor(this->palette().highlight().color());
                palette.setColor(progressbarText->foregroundRole(), color);
                progressbarText->setPalette(palette);
            }else
            {
                QPalette palette = progressbarText->palette();
                QColor color = Qt::black;
                palette.setColor(progressbarText->foregroundRole(), color);
                progressbarText->setPalette(palette);
            }
        }
};

#endif // MYPROGRESSBAR_H
