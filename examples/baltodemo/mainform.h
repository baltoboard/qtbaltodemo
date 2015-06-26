#ifndef MAINFORM_H
#define MAINFORM_H
//---------------------------------------------------------------------------------------------
#include <QtGui>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#endif

#include "flickable.h"
#include "ui_patientcare.h"

//---------------------------------------------------------------------------------------------
#define TIMEOUT 30
//---------------------------------------------------------------------------------------------
namespace Ui {
class mainForm;
}

enum{
    IntroPage = 0,  TachometerPage, ThermometerPage,AmperemeterPage, GraphsPage,
    PatientCarePage,
    ButtonPage,

    NextPage,PrevPage,

    AdvancedPage,//9
    MyTablePage, KineticListPage, SlidePage, TrivalImageSlideShowPage, page14, ExitPage
};
//---------------------------------------------------------------------------------------------
class QtBasicGraph;
class PatientCareController;
class QSlide;
class FluidLauncher;
class QtSvgDialGauge;
class MyPictureSlideShow;
//---------------------------------------------------------------------------------------------
class mainForm : public QWidget
{
    Q_OBJECT

public:
    explicit mainForm(int num = 0, QWidget *parent = 0);
    ~mainForm();

private:
    Ui::mainForm *ui;

    QWidget* pCurrentPage;
    int currenPage;
    int currentIndexPage;

    QSize fsize;

    QPixmap* pRenesasLogoPix;

    void linkDemoButtons(QWidget* p);
    void destroyPage(int nPage, int nextPage);
    void setSkins();
    void setIndexButtonPage(int npage);

    QVector <QPushButton*> indexBtn;

private:
    Ui::PatientCare uiPatentCare;
    PatientCareController* m_controller;


    void ShowDemoPage(int nPage);

protected:
    QWidget *createIntro(QWidget* pParent = NULL);
    QWidget *createTachometer(QWidget* pParent = NULL);
    QWidget *createThermometer(QWidget* pParent= NULL);
    QWidget *createAmperemeter(QWidget * pParent = NULL);
    QWidget *createGraphs(QWidget * pParent = NULL);
    QWidget *createPatientCare(QWidget * pParent = NULL);        
    QWidget *createButtons(QWidget * pParent = NULL);
    QWidget *createSliders(QWidget * pParent= NULL);
    QWidget *createMyTable(QWidget * pParent = NULL);
    QWidget *createKineticList(QWidget * pParent);
    QWidget *createSlidePage(QWidget * pParent);    
    QWidget *createTrivalImageShowPage(QWidget * pParent = NULL);
    QWidget *createWidgetShowPage(QWidget * pParent);

private:
    void fillStaticGraph(QtBasicGraph *graph);


private slots:
    QWidget *showGraph(int index);
    QWidget *generatorTickSlot();

private:    
    QtBasicGraph *pStaticGraph;
    QTimer *pGraphTimer;
    int m_graph_type;
    int m_graph_tick;

    //Slider e Bottoni e timer per gli strumenti analogici
    QSlider *pSlider;
    QPushButton *pStartButton;
    QTimer *pTimerStrumenti;

    QtSvgDialGauge *pTachometer;
    QtSvgDialGauge *pThermometer;
    QtSvgDialGauge *pAmperemeter;


    char effect,color;
    QSlide* pSlide;


    MyPictureSlideShow* pTrivalImage;

    int checkMemIdx;

protected:
      void timerEvent(QTimerEvent *);

private slots:
    void selectDemoSlot();
    void slider1Update();
    void toggleTimerControl1(bool b);    
    void changeEffectsSlot();
    void changeColorSlot(int);
};

const int randVal[] = {5, 10, 15, 20, 24, 29, 33, 37, 42, 46, 50, 54, 58, 62, 65, 69, 73, 76, 80, 83, 86, 89, 93, 96, 99, 102, 105, 108, 111, 113, 116, 119, 121, 124, 126, 129, 131, 134, 136, 138, 140, 143, 145, 147, 149, 151, 153, 155, 157, 159, 160, 162, 164, 166, 167, 169, 171, 172, 174, 175, 177, 178, 180, 181, 182, 184, 185, 186, 188, 189, 190, 191, 192, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 207, 208, 209, 210, 211, 212, 212, 213, 214, 215, 215, 216, 217, 217, 218, 219, 219, 220, 220, 221, 222, 222, 223, 223, 224, 224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 230, 231, 231, 232, 232, 232, 233, 233, 233, 234, 234, 234, 235, 235, 235, 236, 236, 236, 236, 237, 237, 237, 238, 238, 238, 238, 239, 239, 239, 239, 239, 240, 240, 240, 240, 241, 241, 241, 241, 241, 241, 242, 242, 242, 242, 242, 242, 243, 243, 243, 243, 243, 243, 244, 244, 244, 244, 244, 244, 244, 244, 245, 245, 245, 245, 245, 245, 245, 245, 245, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249};

const unsigned int thermVal[] = {3, 6, 9, 12, 15, 17, 20, 23, 25, 28, 30, 33, 35, 37, 39, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 61, 63, 65, 67, 68, 70, 71, 73, 75, 76, 77, 79, 80, 82, 83, 84, 86, 87, 88, 90, 91, 92, 93, 94, 95, 96, 97, 99, 100, 101, 102, 103, 103, 104, 105, 106, 107, 108, 109, 110, 110, 111, 112, 113, 114, 114, 115, 116, 116, 117, 118, 118, 119, 120, 124, 129, 133, 137, 142, 144, 148, 150, 153, 154, 156, 157, 158, 158, 159, 158, 158, 157, 156, 155, 153, 152, 150, 148, 147, 144, 142, 140, 138, 136, 134, 132, 131, 129, 127, 125, 124, 123, 122, 122, 121, 120, 120, 121, 121, 121, 121, 122, 123, 124, 124, 126, 127, 128, 130, 131, 133, 135, 136, 137, 138, 141, 142, 143, 144, 145, 146, 148, 149, 150, 150, 151, 151, 151, 152, 153, 153, 152, 152, 152, 152, 152, 152, 151, 151, 150, 149, 149, 148, 147, 147, 147, 146, 146, 145, 144, 144, 143, 143, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 143, 143, 143, 144, 145, 145, 145, 146, 146, 147, 147, 147, 148, 148, 148, 149, 149, 149, 149, 150, 150, 150, 150, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 150, 150, 150, 150, 150, 149, 149, 149, 149, 149, 148, 148, 148, 148, 148, 148, 148, 147, 147, 147, 147, 147, 147, 147, 147, 148, 148, 148, 148, 148, 148, 148, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 149, 149};

//-------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------
// Returns a list of two-word color names
static QStringList colorPairs(int max)
{
    // capitalize the first letter
    QStringList colors = QColor::colorNames();
    colors.removeAll("transparent");
    int num = colors.count();
    for (int c = 0; c < num; ++c)
        colors[c] = colors[c][0].toUpper() + colors[c].mid(1);

    // combine two colors, e.g. "lime skyblue"
    QStringList combinedColors;
    for (int i = 0; i < num; ++i)
        for (int j = 0; j < num; ++j)
            combinedColors << QString("%1 %2").arg(colors[i]).arg(colors[j]);

    // randomize it
    colors.clear();
    while (combinedColors.count()) {
        int i = qrand() % combinedColors.count();
        colors << combinedColors[i];
        combinedColors.removeAt(i);
        if (colors.count() == max)
            break;
    }

    return colors;
}

class ColorList : public QWidget, public Flickable
{
    Q_OBJECT

public:
    ColorList(QWidget *parent = 0)
            : QWidget(parent) {
        m_offset = 0;
        m_height = QFontMetrics(font()).height() + 5;
        m_highlight = -1;
        m_selected = -1;

        QStringList colors = colorPairs(999);
        for (int i = 0; i < colors.count(); ++i) {
            QString c = colors[i];
            QString str;
            str.sprintf("%4d", i + 1);
            m_colorNames << (str + "   " + c);

            QStringList duet = c.split(' ');
            m_firstColor << duet[0];
            m_secondColor << duet[1];
        }

        //setAttribute(Qt::WA_OpaquePaintEvent,   true);
        //setAttribute(Qt::WA_NoSystemBackground, true);

        //this->setStyleSheet("background-color: rgb(0, 0, 0, 255);");


        setMouseTracking(true);
        Flickable::setAcceptMouseClick(this);
    }

protected:
    // reimplement from Flickable
    virtual QPoint scrollOffset() const {
        return QPoint(0, m_offset);
    }

    // reimplement from Flickable
    virtual void setScrollOffset(const QPoint &offset) {
        int yy = offset.y();
        if (yy != m_offset) {
            m_offset = qBound(0, yy, m_height * m_colorNames.count() - height());
            update();
        }
    }

protected:
    void paintEvent(QPaintEvent *event) {
        QPainter p(this);
        //p.fillRect(event->rect(), Qt::white);
        int start = m_offset / m_height;
        int y = start * m_height - m_offset;
        if (m_offset <= 0) {
            start = 0;
            y = -m_offset;
        }
        int end = start + height() / m_height + 1;
        if (end > m_colorNames.count() - 1)
            end = m_colorNames.count() - 1;
        for (int i = start; i <= end; ++i, y += m_height) {

            p.setBrush(Qt::NoBrush);
            p.setPen(Qt::black);
            if (i == m_highlight) {
                p.fillRect(0, y, width(), m_height, QColor(0, 64, 128));
                p.setPen(Qt::white);
            }
            if (i == m_selected) {
                p.fillRect(0, y, width(), m_height, QColor(0, 128, 240));
                p.setPen(Qt::white);
            }

            p.setPen(Qt::white);
            p.drawText(m_height + 2, y, width(), m_height, Qt::AlignVCenter, m_colorNames[i]);

            p.setPen(Qt::NoPen);
            p.setBrush(m_firstColor[i]);
            p.drawRect(1, y + 1, m_height - 2, m_height - 2);
            p.setBrush(m_secondColor[i]);
            p.drawRect(5, y + 5, m_height - 11, m_height - 11);
        }
        p.end();
    }

    void keyReleaseEvent(QKeyEvent *event) {
        if (event->key() == Qt::Key_Down) {
            m_offset += 20;
            event->accept();
            update();
            return;
        }
        if (event->key() == Qt::Key_Up) {
            m_offset -= 20;
            event->accept();
            update();
            return;
        }
    }

    void mousePressEvent(QMouseEvent *event) {
        Flickable::handleMousePress(event);
        if (event->isAccepted())
            return;

        if (event->button() == Qt::LeftButton) {
            int y = event->pos().y() + m_offset;
            int i = y / m_height;
            if (i != m_highlight) {
                m_highlight = i;
                m_selected = -1;
                update();
            }
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) {
        Flickable::handleMouseMove(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) {
        Flickable::handleMouseRelease(event);
        if (event->isAccepted())
            return;

        if (event->button() == Qt::LeftButton) {
            m_selected = m_highlight;
            event->accept();
            update();
        }
    }

private:
    int m_offset;
    int m_height;
    int m_highlight;
    int m_selected;
    QStringList m_colorNames;
    QList<QColor> m_firstColor;
    QList<QColor> m_secondColor;
};
#endif // MAINFORM_H
