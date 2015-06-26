#ifndef MYPICTURESLIDESHOW_H
#define MYPICTURESLIDESHOW_H

#include <QWidget>

class MyPictureSlideShow : public QWidget
{
    Q_OBJECT
private:
    int currIdx;
    QList<QString> list;
    QImage* pCurrImage;
    int timerIdx;

    void appendImage(QString str){
        list.append(str);
    }
    void LoadImage(int n);

public:
    explicit MyPictureSlideShow(QWidget *parent = 0);
    ~MyPictureSlideShow();

    void Stop();
signals:

protected:
    void paintEvent(QPaintEvent* ev);
    void timerEvent(QTimerEvent *ev);

public slots:
};

#endif // MYPICTURESLIDESHOW_H
