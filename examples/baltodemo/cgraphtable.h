#ifndef CGRAPHTABLE_H
#define CGRAPHTABLE_H
//-----------------------------------------------------------------------------------------------------------------------------------
#include <QWidget>
#include <QtGui>
//-----------------------------------------------------------------------------------------------------------------------------------
#define MAX_POINT 255
//-----------------------------------------------------------------------------------------------------------------------------------
class cGraphTable : public QWidget
{
    Q_OBJECT

public:
    enum PathMode { CurveMode, LineMode };

    explicit cGraphTable(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);
    void paint(QPainter *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void timerEvent(QTimerEvent *e);
    bool event(QEvent *e);
    void resizeEvent(QResizeEvent *e);


public:
    QSize sizeHint()        const { return QSize(m_pointCount*(500/16), 500); }
    qreal realPenWidth()    const { return m_penWidth; }
    void  setRealPenWidth(qreal penWidth) { m_penWidth = penWidth; update(); }

    int getNumPoint(){
        return(m_pointCount);
    }

signals:
    void clicked();

public slots:
    void setPenWidth(int penWidth) { m_penWidth = penWidth / 10.0; update(); }

    void setFlatCap()       { m_capStyle = Qt::FlatCap; update(); }
    void setSquareCap()     { m_capStyle = Qt::SquareCap; update(); }
    void setRoundCap()      { m_capStyle = Qt::RoundCap; update(); }

    void setBevelJoin()     { m_joinStyle = Qt::BevelJoin; update(); }
    void setMiterJoin()     { m_joinStyle = Qt::MiterJoin; update(); }
    void setRoundJoin()     { m_joinStyle = Qt::RoundJoin; update(); }

    void setCurveMode()     { m_pathMode = CurveMode; update(); }
    void setLineMode()      { m_pathMode = LineMode; update(); }

    void setSolidLine()     { m_penStyle = Qt::SolidLine; update(); }
    void setDashLine()      { m_penStyle = Qt::DashLine; update(); }
    void setDotLine()       { m_penStyle = Qt::DotLine; update(); }
    void setDashDotLine()   { m_penStyle = Qt::DashDotLine; update(); }
    void setDashDotDotLine(){ m_penStyle = Qt::DashDotDotLine; update(); }
    void setCustomDashLine(){ m_penStyle = Qt::NoPen; update(); }

private:
    void initializePoints();
    void updatePoints();
    void findActivePoint(QPoint pos);

    //
    // Mie
    //

    int m_deltaX;
    int m_deltaY;
    int m_bottomSpace;
    //int m_maxPoint;
    int m_borderLineWidth;
    bool m_moveVertical;
    double m_vertScale;

    int m_max;
    int m_min;

    int ClientWidth;
    int ClientHeight;
    float m_deltaPoint;


    bool m_drawMode;


    PathMode m_pathMode;

    bool m_wasAnimated;

    qreal m_penWidth;

    int timerPushCurvePointIdx;

    int m_pointCount;
    int m_pointSize;
    int m_activePoint;
    int m_value[MAX_POINT];
    bool m_pushValue[MAX_POINT];

    QVector<QPointF> m_points;
    QVector<QPointF> m_vectors;

    Qt::PenJoinStyle m_joinStyle;
    Qt::PenCapStyle m_capStyle;

    Qt::PenStyle m_penStyle;

    bool m_smallScreen;
    QPoint m_mousePress;
    bool m_mouseDrag;

    QHash<int, int> m_fingerPointMapping;


    void setPointsize();

    void setClientParameters();

    QPoint clipToClientArea(QPoint &p);
    int GetXPoint(int i);
    int GetYPoint(int val);

    int GetXVal(int pixel);
    int GetYVal(int pixel);

public:
    void setDeltaX(int x)           { m_deltaX = x; }
    void setDeltaY(int y)           { m_deltaY = y; }
    //void setMaxPoint(int n)         { m_maxPoint = n; }
    void setBorderLineWidth(int n)  { m_borderLineWidth = n;}
    void setBottomSpace(int n)      { m_bottomSpace = n;}

    void setNumPoint(int n, int nim = 0, int max = 127);
    void setValue(int idx, int val);
    int  getValue(int idx);

    void setPushValue(int posIdx);

    int getControlPointSize(){
        return(m_pointSize);
    }
    void setControlPointSize(int val){
        m_pointSize = val;
        this->update();
    }

signals:
    void onValueChange (int idx, int val);
    void onValueChanged(int idx, int val);

};

#endif // CGRAPHTABLE_H
