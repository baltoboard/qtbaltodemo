#include <QApplication>

#include "cgraphtable.h"
//-----------------------------------------------------------------------------------------------------------------------------
cGraphTable::cGraphTable(QWidget *parent) :
    QWidget(parent)
{   

    m_deltaX        = 20;
    m_deltaY        = 20;
    m_bottomSpace   = 10;
    m_moveVertical  = true;

    timerPushCurvePointIdx = -1;

    //m_maxPoint      = 16;
    m_max           = 127;
    m_min           = 0;
    m_pointCount    = 16;

    m_drawMode      = false;

    setClientParameters();

    m_borderLineWidth=2;

    m_pointSize     = 10;
    m_activePoint   = -1;

    m_capStyle      = Qt::FlatCap;
    m_joinStyle     = Qt::BevelJoin;
    //m_pathMode      = CurveMode;
    m_pathMode      = LineMode;
    m_penWidth      = 1;
    m_penStyle      = Qt::SolidLine;

    m_wasAnimated   = true;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMaximumSize(16777215,16777215);

    //setAttribute(Qt::WA_AcceptTouchEvents);

    setNumPoint(128);

    for(int i=0;i<MAX_POINT;i++){
        m_value[i]      = 0;
        m_pushValue[i]  = false;
    }
    this->setFocusPolicy(Qt::StrongFocus);
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::setNumPoint(int n, int min, int max){

    if ((n>2)&&(n < MAX_POINT)){

        m_pointCount = n;
        m_deltaPoint = this->ClientWidth/(n-1);

        m_max   = max;
        m_min   = min;
        m_vertScale  = this->ClientHeight/((max-min)*1.0);

        m_points.clear();
        m_vectors.clear();

        setPointsize();
    }

}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::setPointsize(){
/*
    if(m_pointCount > 0){

        m_pointSize = ClientWidth/(m_pointCount*2);

        if ( m_pointSize < 1){
            m_pointSize = 1;
        }else{
            if(m_pointSize > 10){
                m_pointSize = 10;
            }
        }
    }

    qDebug()<< "m_pointSize:" << m_pointSize;
*/
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::setClientParameters(){

    setGeometry(0,0,this->parentWidget()->width(),this->parentWidget()->height() );

    ClientHeight    = this->height()- (m_deltaY*2)-m_bottomSpace;
    ClientWidth     = this->width() - (m_deltaX*2);

    setPointsize();

}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::setValue(int idx, int val){
    if(idx < MAX_POINT){
        m_value[idx] = val;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
int cGraphTable::getValue(int idx){
    if(idx < MAX_POINT){
        return(m_value[idx]);
    }
    return(0);
}
//----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::setPushValue(int posIdx){
    m_pushValue[posIdx] = true;
    this->update();

    if( timerPushCurvePointIdx == -1 ){
        timerPushCurvePointIdx = this->startTimer(800);
    }
}

//----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    //painter.fillRect(e->rect(),Qt::white);
    paint(&painter);
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::paint(QPainter *painter)
{
    if (m_points.isEmpty()){
        initializePoints();
    }

    painter->setRenderHint(QPainter::Antialiasing);

    QPalette pal = palette();
    painter->setPen(Qt::NoPen);

    // Construct the path
    QPainterPath path;
    path.moveTo(m_points.at(0));

    if (m_pathMode == LineMode) {
        for (int i=1; i<m_points.size(); ++i) {
            path.lineTo(m_points.at(i));
        }
    } else {
        int i=1;
        while (i + 2 < m_points.size()) {
            path.cubicTo(m_points.at(i), m_points.at(i+1), m_points.at(i+2));
            i += 3;
        }
        while (i < m_points.size()) {
            path.lineTo(m_points.at(i));
            ++i;
        }
    }

    // Draw the path
    {
        QColor lg = Qt::red;

        // The "custom" pen
        if (m_penStyle == Qt::NoPen) {

            QPainterPathStroker stroker;
            stroker.setWidth(m_penWidth);
            stroker.setJoinStyle(m_joinStyle);
            stroker.setCapStyle(m_capStyle);

            QVector<qreal> dashes;
            qreal space = 4;
            dashes << 1 << space
                   << 3 << space
                   << 9 << space
                   << 27 << space
                   << 9 << space
                   << 3 << space;
            stroker.setDashPattern(dashes);
            QPainterPath stroke = stroker.createStroke(path);
            painter->fillPath(stroke, lg);

        } else {

            QPen pen(lg, m_penWidth, m_penStyle, m_capStyle, m_joinStyle);
            painter->strokePath(path, pen);

        }
    }

    if (1) {
        //
        // Draw the control points
        //

        painter->setPen(QColor(50, 100, 120, 200));
        painter->setBrush(QColor(200, 200, 210, 120));

        bool pal = false;

        for (int i=0; i<m_points.size(); ++i) {

            if((i == m_activePoint) && (pal == false)){
                pal = true;
                painter->setPen(QColor(250, 100, 120, 200));
                painter->setBrush(QColor(200, 0, 0, 120));
            }else{
                if(pal == true){
                    pal = false;
                    painter->setPen(QColor(50, 100, 120, 200));
                    painter->setBrush(QColor(200, 200, 210, 120));
                }
            }                        

            if(m_pushValue[i] == true){
                //qDebug() << "Blue:" << i;
                painter->setBrush(Qt::red);
                painter->setBrush(Qt::blue);
                pal = true;
            }

            QPointF pos = m_points.at(i);
            painter->drawEllipse(QRectF(pos.x() - m_pointSize,
                                       pos.y()  - m_pointSize,
                                       m_pointSize*2, m_pointSize*2));
        }


        painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawPolyline(m_points);
    }



    //
    // Bordo
    //

    if (m_borderLineWidth != 0) {

        QColor lg = Qt::black;
        QPen pen(lg, m_borderLineWidth, m_penStyle, m_capStyle, m_joinStyle);
        pen.setWidth(m_borderLineWidth);
        painter->setPen(pen);

        QRect rec(m_deltaX, m_deltaY,  ClientWidth + (m_borderLineWidth) * 2,	ClientHeight - m_borderLineWidth) ;

        if (rec.isValid()) {
            painter->drawRect(rec);
        }
    }

}
//----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::initializePoints()
{    
    m_points.clear();
    m_vectors.clear();

    m_deltaPoint = this->ClientWidth/((m_pointCount-1)*1.0);
    m_vertScale  = this->ClientHeight/(((m_max+1)-m_min)*1.0);

    for (int i=0; i<m_pointCount; i++) {

        int x = GetXPoint(i);
        int y = GetYPoint(m_value[i]);
        m_vectors << QPointF(x, y);
        m_points  << QPointF(x, y);
    }
}
//----------------------------------------------------------------------------------------------------------------------------
QPoint cGraphTable::clipToClientArea(QPoint &p){

    if(p.x() < m_deltaX){
        p.setX(m_deltaX);
    }

    if(p.x() > (m_deltaX + ClientWidth)){
        p.setX(m_deltaX + ClientWidth);
    }

    if ( p.y() < m_deltaY){
        p.setY(m_deltaY);
    }
    if ( p.y() > (ClientHeight+m_deltaY - m_borderLineWidth) ){
        p.setY((ClientHeight+m_deltaY - m_borderLineWidth));
    }

    return(p);
}
//----------------------------------------------------------------------------------------------------------------------------
int cGraphTable::GetXPoint(int i){
    return(m_deltaPoint*i + m_deltaX);
}
//----------------------------------------------------------------------------------------------------------------------------
int cGraphTable::GetYPoint(int val){
    return( (ClientHeight-val*m_vertScale) + m_deltaY - m_borderLineWidth);
}
//----------------------------------------------------------------------------------------------------------------------------
int cGraphTable::GetXVal(int /*pixelx*/){
    return(0);
}
//----------------------------------------------------------------------------------------------------------------------------
int cGraphTable::GetYVal(int pixely){

    int val = (m_deltaY - m_borderLineWidth + ClientHeight - pixely)/m_vertScale;

    if ( val > m_max){
        val = m_max;
    }

    if ( val < m_min){
        val = m_min;
    }

    return( val );
}
//----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::updatePoints()
{
    qreal pad = 10;
    qreal left = pad;
    qreal right = width() - pad;
    qreal top = pad;
    qreal bottom = height() - pad;

    Q_ASSERT(m_points.size() == m_vectors.size());

    for (int i=0; i<m_points.size(); ++i) {
        QPointF pos = m_points.at(i);
        QPointF vec = m_vectors.at(i);
        pos += vec;
        if (pos.x() < left || pos.x() > right) {
            vec.setX(-vec.x());
            pos.setX(pos.x() < left ? left : right);
        } if (pos.y() < top || pos.y() > bottom) {
            vec.setY(-vec.y());
            pos.setY(pos.y() < top ? top : bottom);
        }
        m_points[i]  = pos;
        m_vectors[i] = vec;
    }
    update();
}
//-----------------------------------------------------------------------------------------------------------------------------
//                                                  EVENTS
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::resizeEvent(QResizeEvent */*e*/){
    //qDebug() << "resizeEvent";
    setClientParameters();
    initializePoints();
    this->update();
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::findActivePoint(QPoint pos){


    pos.setX(pos.x()+10);


    int old = m_activePoint;

    m_activePoint  = -1;
    qreal distance = -1;

    for (int i=0; (i<m_points.size()) && (m_activePoint == -1); ++i) {

        QPoint p1 = QPoint( m_points.at(i).x()-m_deltaX, pos.y() );

        qreal   d = QLineF(pos, p1).length();

        if ((distance < 0 && d < 8 * m_pointSize) || d < distance) {

            distance = d;
            m_activePoint = i;

            if((old != -1) && (old != m_activePoint)){

                int val = GetYVal(m_points[old].y());
                m_value[old] = val;
                emit onValueChanged (old, val);
            }
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::mousePressEvent(QMouseEvent *e)
{
    //qDebug() << "mousePressEvent :" << e->pos();
    if (!m_fingerPointMapping.isEmpty())
        return;

    findActivePoint(e->pos());

    if (m_activePoint != -1) {
        //qDebug() << "m_activePoint:" << m_activePoint;
        //m_wasAnimated = m_timer.isActive();
        //setAnimation(false);
        mouseMoveEvent(e);
    }

    // If we're not running in small screen mode, always assume we're dragging
    m_mouseDrag = !m_smallScreen;
    m_mousePress = e->pos();
    e->accept();
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::mouseMoveEvent(QMouseEvent *e)
{

    //qDebug() << "mouseMoveEvent :" << e->pos();

    if (!m_fingerPointMapping.isEmpty())
        return;

    if(m_drawMode){
        findActivePoint(e->pos());
        m_mousePress = e->pos();
    }

    // If we've moved more then 25 pixels, assume user is dragging
    if (!m_mouseDrag && QPoint(m_mousePress - e->pos()).manhattanLength() > 25)
        m_mouseDrag = true;

    if (m_mouseDrag && m_activePoint >= 0 && m_activePoint < m_points.size()) {

        QPoint p = e->pos();

        if(m_moveVertical){
            p.setX(this->GetXPoint(m_activePoint));
        }

        p = clipToClientArea(p);

        m_points[m_activePoint] = p;
        update();

        //qDebug() << " Val:" << GetYVal(p.y());

        emit onValueChange (m_activePoint, GetYVal(p.y()));
    }

    e->accept();
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::mouseReleaseEvent(QMouseEvent *e)
{

    //qDebug() << "mouseReleaseEvent :" << e->pos();

    if (!m_fingerPointMapping.isEmpty())
        return;

    if (m_activePoint != -1){
        int val = GetYVal(m_points[m_activePoint].y());
        m_value[m_activePoint] = val;
        emit onValueChanged (m_activePoint, val);
    }

    m_activePoint = -1;
    //setAnimation(m_wasAnimated);

    if (!m_mouseDrag && m_smallScreen)
        emit clicked();

    this->update();
    e->accept();
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::timerEvent(QTimerEvent *e)
{

    if(e->timerId() == timerPushCurvePointIdx){
        this->killTimer(timerPushCurvePointIdx);
        timerPushCurvePointIdx = -1;

        for(int i=0;i<MAX_POINT;i++){
            m_pushValue[i] = false;
        }
        this->update();
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::keyReleaseEvent(QKeyEvent* e){


    m_drawMode = false;
    //e->accept();
}
//-----------------------------------------------------------------------------------------------------------------------------
void cGraphTable::keyPressEvent(QKeyEvent* e){

    //qDebug() << e->key();

    if (e->modifiers()==Qt::ShiftModifier)
       {
         //qDebug() << "SHIFT";
         m_drawMode = true;
       }

    e->accept();
}
//-----------------------------------------------------------------------------------------------------------------------------
bool cGraphTable::event(QEvent *e)
{

    //qDebug() << e->type();

    //bool touchBegin = false;

    switch (e->type()) {
    case QEvent::WindowDeactivate:
    case QEvent::WindowActivate:
    case QEvent::Enter:
        this->setGeometry(0,0,this->parentWidget()->width(),this->parentWidget()->height() );
        break;
/*
    case QEvent::TouchBegin:
        touchBegin = true;
    case QEvent::TouchUpdate:
        {
            const QTouchEvent *const event = static_cast<const QTouchEvent*>(e);
            const QList<QTouchEvent::TouchPoint> points = event->touchPoints();
            foreach (const QTouchEvent::TouchPoint &touchPoint, points) {
                const int id = touchPoint.id();
                switch (touchPoint.state()) {
                case Qt::TouchPointPressed:
                    {
                        // find the point, move it
                        QSet<int> activePoints = QSet<int>::fromList(m_fingerPointMapping.values());
                        int activePoint = -1;
                        qreal distance = -1;
                        const int pointsCount = m_points.size();
                        for (int i=0; i<pointsCount; ++i) {
                            if (activePoints.contains(i))
                                continue;

                            qreal d = QLineF(touchPoint.pos(), m_points.at(i)).length();
                            if ((distance < 0 && d < 12 * m_pointSize) || d < distance) {
                                distance = d;
                                activePoint = i;
                            }
                        }
                        if (activePoint != -1) {
                            m_fingerPointMapping.insert(touchPoint.id(), activePoint);
                            m_points[activePoint] = touchPoint.pos();
                        }
                    }
                    break;
                case Qt::TouchPointReleased:
                    {
                        // move the point and release
                        QHash<int,int>::iterator it = m_fingerPointMapping.find(id);
                        m_points[it.value()] = touchPoint.pos();
                        m_fingerPointMapping.erase(it);
                    }
                    break;
                case Qt::TouchPointMoved:
                    {
                        // move the point
                        const int pointIdx = m_fingerPointMapping.value(id, -1);
                        if (pointIdx >= 0)
                            m_points[pointIdx] = touchPoint.pos();
                    }
                    break;
                default:
                    break;
                }
            }
            if (m_fingerPointMapping.isEmpty()) {
                e->ignore();
                return false;
            } else {
                if (touchBegin) {
                    //m_wasAnimated = m_timer.isActive();
                    //setAnimation(false);
                }
                update();
                return true;
            }
        }
        break;
    case QEvent::TouchEnd:
        if (m_fingerPointMapping.isEmpty()) {
            e->ignore();
            return false;
        }
        m_fingerPointMapping.clear();
        //setAnimation(m_wasAnimated);
        return true;
        break;
*/

    default:
        break;
    }
    return QWidget::event(e);
}
