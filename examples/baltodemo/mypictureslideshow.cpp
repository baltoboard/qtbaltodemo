#include <QPainter>
#include <QTimerEvent>
#include "mypictureslideshow.h"
//-----------------------------------------------------------------------------------------------------
MyPictureSlideShow::MyPictureSlideShow(QWidget *parent) : QWidget(parent)
{

    currIdx     = 0;
    pCurrImage  = 0;

    this->appendImage(":/slides/images4.jpg");
    this->appendImage(":/slides/images5.jpg");
    this->appendImage(":/slides/images6.jpg");
    this->appendImage(":/slides/images7.jpg");
    this->appendImage(":/slides/images8.jpg");

    LoadImage(0);
    timerIdx = this->startTimer(2000);
}

//-----------------------------------------------------------------------------------------------------
void MyPictureSlideShow::Stop(){
    this->killTimer(timerIdx);
}
//-----------------------------------------------------------------------------------------------------
MyPictureSlideShow::~MyPictureSlideShow()
{
    this->killTimer(timerIdx);
    list.clear();
    delete pCurrImage;
}
//-----------------------------------------------------------------------------------------------------
void MyPictureSlideShow::timerEvent(QTimerEvent *ev){

    if(ev->timerId() == timerIdx){
        LoadImage(currIdx+1);
        this->update();
    }
}

//-----------------------------------------------------------------------------------------------------
void MyPictureSlideShow::LoadImage(int n){

    if (n >= list.count())
        n = 0;

    if ( n < list.count()){
        if( pCurrImage != NULL)
            delete pCurrImage;
        pCurrImage = new QImage(list[n]);
        currIdx = n;
    }
}
//-----------------------------------------------------------------------------------------------------
void MyPictureSlideShow::paintEvent(QPaintEvent* ev){

    if( pCurrImage != NULL ){

        double widgetWidth = this->width();
        double widgetHeight = this->height();
        QRectF target(0, 0, widgetWidth, widgetHeight);

        *pCurrImage = pCurrImage->scaled(rect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        double imageSizeWidth = static_cast<double>(pCurrImage->width());
        double imageSizeHeight = static_cast<double>(pCurrImage->height());
        QRectF source(0.0, 0.0, imageSizeWidth, imageSizeHeight);

        int deltaX = 0;
        int deltaY = 0;
        if(source.width() < target.width())
          deltaX = target.width() - source.width();
        else
          deltaX = source.width() - target.width();

        if(source.height() < target.height())
          deltaY = target.height() - source.height();
        else
          deltaY = source.height() - target.height();

        QPainter painter(this);
        painter.translate(deltaX / 2, deltaY / 2);

        painter.drawImage(source, *pCurrImage);

    }

}

