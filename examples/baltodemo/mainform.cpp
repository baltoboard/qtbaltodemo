#include <Qt5WayButton>
#include <QtBasicDialGauge>
#include <QtBasicGraph>
#include <QtMultiSlider>
#include <QtScrollDial>
#include <QtScrollWheel>
#include <QtSvgButton>
#include <QtSvgDialGauge>
#include <QtSvgSlideSwitch>
#include <QtSvgToggleSwitch>

#include "cgraphtable.h"
#include "qslide.h"

#include "mypictureslideshow.h"

#include <math.h>
#include "patientcarecontroller.h"
#include "allocator.h"
//---------------------------------------------------------------------------------------------
extern bool gCloseAndReOpen;
//---------------------------------------------------------------------------------------------
#include "mainform.h"
#include "ui_mainform.h"
//---------------------------------------------------------------------------------------------
bool m_embedded     = true;
int guageWidgetSize1;
//---------------------------------------------------------------------------------------------
#ifndef TRUE
#define TRUE true
#endif
//---------------------------------------------------------------------------------------------
#ifndef FALSE
#define FALSE false
#endif
//---------------------------------------------------------------------------------------------
enum FLAG {
UP,
DOWN,
STAY
};
//---------------------------------------------------------------------------------------------
mainForm::mainForm(int num, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainForm)
{
    ui->setupUi(this);

    pRenesasLogoPix = NULL;
    pTimerStrumenti = NULL;
    pTachometer     = NULL;
    pThermometer    = NULL;
    pAmperemeter    = NULL;
    pStartButton    = NULL;
    pSlider         = NULL;
    pStaticGraph    = NULL;
    pGraphTimer     = NULL;

    fsize = frameSize();
    guageWidgetSize1 = 800;

    pCurrentPage = NULL;
    linkDemoButtons(this);

    currenPage = -1;

    setIndexButtonPage(0);

    ShowDemoPage(num);

    checkMemIdx = -1;

    if (pMyAllocator != NULL){
        checkMemIdx = this->startTimer(500);
    }

    ui->pushButton_1->setFocus();
}
//---------------------------------------------------------------------------------------------
mainForm::~mainForm()
{
    delete ui;
}
//---------------------------------------------------------------------------------------------
void mainForm::setIndexButtonPage(int npage){

    if(npage == 0){

        for(int i=0;i<indexBtn.count();i++){

            QPushButton* p = indexBtn.at(i);
            QVariant v = p->property("idx");
            if( v.isValid() ){
                switch(v.toInt()){

                case 0:
                    p->setText("Index");
                    break;
                case 1:
                    p->setText("Tachometer");
                    break;
                case 2:
                    p->setText("Thermom.");
                    break;
                case 3:
                    p->setText("Amperem.");
                    break;
                case 4:
                    p->setText("Graph");
                    break;
                case 5:
                    p->setText("Patient Care");
                    break;
                case 6:
                    p->setText("G.Buttons");
                    break;
                case 7:
                    p->setText("Next >>");
                    break;

                }
            }
        }
        currentIndexPage = 0;
    }else{
        if(npage == 1){

            for(int i=0;i<indexBtn.count();i++){

                QPushButton* p = indexBtn.at(i);
                QVariant v = p->property("idx");
                if( v.isValid() ){
                    switch(v.toInt()){

                    case 0:
                        p->setText("<< Prev");
                        break;
                    case 1:
                        p->setText("Advanced");
                        break;
                    case 2:
                        p->setText("Table");
                        break;
                    case 3:
                        p->setText("Kinetic L.");
                        break;
                    case 4:
                        p->setText("Scroll B.");
                        break;
                    case 5:
                        p->setText("T.Show");
                        break;
                    case 6:
                        p->setText("-");
                        break;
                    case 7:
                        p->setText("Exit");
                        break;

                    }
                }
            }
            currentIndexPage = 1;
        }
    }
}

//---------------------------------------------------------------------------------------------
void mainForm::linkDemoButtons(QWidget* p){


    foreach(QWidget *pw, p->findChildren<QWidget*>()){

        //qDebug() << pw;

        if(pw->isWidgetType()){

            QPushButton* pb = (QPushButton*)pw;
            QVariant v = pb->property("demoBtn");
            if(v.isValid()){
                connect(pb,SIGNAL(clicked()),this,SLOT(selectDemoSlot()), Qt::QueuedConnection);
                indexBtn.append(pb);
            }            
        }

        linkDemoButtons(pw);

    }
}
//---------------------------------------------------------------------------------------------
void mainForm::destroyPage(int nPage, int nextPage){

    switch(nPage){
        case IntroPage:
            if(pRenesasLogoPix != NULL){
                delete pRenesasLogoPix;
                pRenesasLogoPix = NULL;
            }
            break;

        case TachometerPage:
        case ThermometerPage:
        case AmperemeterPage:
            if(pTimerStrumenti != NULL){
                pTimerStrumenti->stop();
                delete pTimerStrumenti;
                pTimerStrumenti = NULL;
            }

            if(pTachometer != NULL){
                delete pTachometer;
                pTachometer = NULL;
            }

            if(pThermometer != NULL){
                delete pThermometer;
                pThermometer = NULL;
            }

            if(pAmperemeter  != NULL){
                delete pAmperemeter;
                pAmperemeter = NULL;
            }

            if(pStartButton != NULL){
                delete pStartButton;
                pStartButton = NULL;
            }

            if(pSlider != NULL){
                delete pSlider;
                pSlider = NULL;
            }
            break;

        case GraphsPage:

            if(pGraphTimer != NULL){
                pGraphTimer->stop();
                delete pGraphTimer;
                pGraphTimer = NULL;
            }
            if(pStaticGraph != NULL){
                delete pStaticGraph;
                pStaticGraph= NULL;
            }
            break;

        case TrivalImageSlideShowPage:
            pTrivalImage->Stop();
            break;

    }

    delete pCurrentPage;
    pCurrentPage = NULL;

    QCoreApplication::processEvents();

    if(gCloseAndReOpen == true){

        QString name = QString("/tmp/%1.blt").arg(nextPage);

        QFile file(name);

        if(file.open(QIODevice::WriteOnly)){
            file.close();
            this->close();
        }
    }
}
//---------------------------------------------------------------------------------------------
void mainForm::ShowDemoPage(int nPage){

    switch(nPage){
        case IntroPage: //0
            if(currenPage != -1){
                destroyPage(currenPage,nPage);

            }
            pCurrentPage = createIntro(ui->mainWidget);
            pCurrentPage->show();
            currenPage = IntroPage;
        break;
        case TachometerPage://1
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createTachometer(ui->mainWidget);
            pCurrentPage->show();
            currenPage = TachometerPage;
            break;
        case ThermometerPage://2
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createThermometer(ui->mainWidget);
            pCurrentPage->show();
            currenPage = ThermometerPage;
            break;
        case AmperemeterPage://3
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createAmperemeter(ui->mainWidget);
            pCurrentPage->show();
            currenPage = AmperemeterPage;
            break;
        case GraphsPage://4
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createGraphs(ui->mainWidget);
            pCurrentPage->show();
            currenPage = GraphsPage;
            break;
        case PatientCarePage://5
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createPatientCare(ui->mainWidget);
            pCurrentPage->show();
            currenPage = PatientCarePage;
            break;
        case ButtonPage://6
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createButtons(ui->mainWidget);
            pCurrentPage->show();
            currenPage = ButtonPage;
            break;

        case NextPage://7
            setIndexButtonPage(1);
            break;
        case PrevPage://8
            setIndexButtonPage(0);
            break;

        case AdvancedPage:
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createSliders(ui->mainWidget);
            pCurrentPage->show();
            currenPage = ButtonPage;
            break;

       case MyTablePage:
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createMyTable(ui->mainWidget);
            pCurrentPage->show();
            currenPage = MyTablePage;
            break;

        case KineticListPage:
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createKineticList(ui->mainWidget);
            pCurrentPage->show();
            currenPage = KineticListPage;
            break;

       case SlidePage:
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createSlidePage(ui->mainWidget);
            pCurrentPage->show();
            currenPage = SlidePage;
            break;

        case TrivalImageSlideShowPage:
            if(currenPage != -1){
                destroyPage(currenPage,nPage);
            }
            pCurrentPage = createTrivalImageShowPage(ui->mainWidget);
            pCurrentPage->show();
            currenPage = TrivalImageSlideShowPage;
            break;

        case ExitPage:
            this->close();
            break;
    }
}
//---------------------------------------------------------------------------------------------
void mainForm::selectDemoSlot(){

    //qDebug() << "selectDemoSlot()";

    QPushButton* p = (QPushButton*) sender();
    QVariant v = p->property("idx");

    if( v.isValid() ){
        int nBtn = v.toInt();

        int nDemoPage = v.toInt() + (currentIndexPage * 8);
        ShowDemoPage(nDemoPage);
    }
}

//---------------------------------------------------------------------------------------------
//                                  Tachometer
//---------------------------------------------------------------------------------------------
QWidget *mainForm::createIntro(QWidget* pParent)
{
    QWidget *page           = new QWidget(pParent);
    page->setGeometry(0,0,pParent->width(), pParent->height());
    QLabel*  pLabel         = new QLabel(page);
    pRenesasLogoPix         = new QPixmap(":/Renesas-Logo.png");
    pLabel->setGeometry(0,0,page->width(),page->height());
    pLabel->setPixmap(*pRenesasLogoPix);
    pLabel->setScaledContents(true);
    return page;
}
//---------------------------------------------------------------------------------------------
//                                  Tachometer
//---------------------------------------------------------------------------------------------
QWidget *mainForm::createTachometer(QWidget* pParent)
{
    QWidget *page       = new QWidget(pParent);
    QVBoxLayout *toplay = new QVBoxLayout(page);

    QWidget *group      = new QGroupBox(tr("Tachometer"), page);

    QGridLayout *grid   = new QGridLayout(group);
    QGridLayout *boxgrid= new QGridLayout(group);

    toplay->addWidget(group);

    // Tachometer

    pTachometer = new QtSvgDialGauge(group);
    pTachometer->setSkin("Tachometer");
    pTachometer->setNeedleOrigin(0.486, 0.466);
    pTachometer->setMinimum(0);
    pTachometer->setMaximum(360);
    pTachometer->setStartAngle(-130);
    pTachometer->setEndAngle(133);
    pTachometer->setValue(0);
    pTachometer->setMaximumSize(500,300);
    pTachometer->setMinimumSize(500,300);

    pSlider = new QSlider(Qt::Horizontal, group);
    pSlider->setRange(0, 360);
    pSlider->setValue(0);
    pSlider->setProperty("isEmbedded", m_embedded);
    pSlider->setMinimumSize(250,50);
    connect(pSlider, SIGNAL(valueChanged(int)), pTachometer, SLOT(setValue(int)));

    pStartButton = new QPushButton(tr("Stop Automation"), group);
    pStartButton->setObjectName("ToggleTimer");
    pStartButton->setCheckable(true);
    pStartButton->setChecked(true);
    connect(pStartButton, SIGNAL(toggled(bool)), this, SLOT(toggleTimerControl1(bool)));

    boxgrid->addWidget(pSlider,           0, 0);
    boxgrid->addWidget(pStartButton,  2, 0);

    grid->addLayout(boxgrid,     0, 0, Qt::AlignCenter);
    grid->addWidget(pTachometer, 0, 1, Qt::AlignHCenter);

    pTimerStrumenti = new QTimer();
    connect(pTimerStrumenti, SIGNAL(timeout()), this, SLOT(slider1Update()));
    pTimerStrumenti->start(TIMEOUT);

    return page;
}
//----------------------------------------------------------------------------------
void mainForm::toggleTimerControl1(bool b) {

    pStartButton->setText(b ? tr("Stop Automation") : tr("Start Automation"));

    if(pTimerStrumenti != NULL){
        switch (b){
        case TRUE:
            pTimerStrumenti->start(TIMEOUT);
            break;
        case FALSE:
            pTimerStrumenti->stop();
        }
    }
}
//-------------------------------------------------------------------------
void mainForm::slider1Update() {

    static FLAG flag = UP;
    static int intCount = 0;
    int val = pSlider->value();

    switch (flag){
    case UP:
        if (intCount < 255){
            pSlider->setValue(randVal[intCount++]);
        }else{
            flag = DOWN;
            intCount = 0;
        }
        break;
    case DOWN:
        if (val >= 2){
            val = val-1;
            pSlider->setValue(val);
        } else {
            flag = STAY;
        }
        break;
    case STAY:
        if (intCount < 80){
            pSlider->setValue(val);
            intCount++;
            break;
        }
    default:
        flag = UP;
        intCount = 0;
    }
}
//--------------------------------------------------------------------------------------------
//                          Thermometer
//--------------------------------------------------------------------------------------------
QWidget *mainForm::createThermometer(QWidget* pParent)
{
    QWidget *page       = new QWidget(pParent);
    QVBoxLayout *toplay = new QVBoxLayout(page);

    QWidget *group      = new QGroupBox(tr("Thermometer"), page);
    QGridLayout *grid   = new QGridLayout(group);
    QGridLayout *boxgrid= new QGridLayout(group);

    toplay->addWidget(group);

    // Thermometer
    pThermometer = new QtSvgDialGauge(group);
    pThermometer->setSkin("Thermometer");
    pThermometer->setNeedleOrigin(0.456, 0.459);
    pThermometer->setMinimum(0);
    pThermometer->setMaximum(360);
    pThermometer->setStartAngle(-90);
    pThermometer->setEndAngle(150);
    pThermometer->setValue(0);
    pThermometer->setMaximumSize(500,300);
    pThermometer->setMinimumSize(500,300);

    pSlider = new QSlider(Qt::Horizontal, group);
    pSlider->setRange(0, 360);
    pSlider->setValue(0);
    pSlider->setProperty("isEmbedded", m_embedded);
    pSlider->setMinimumSize(250,50);
    connect(pSlider, SIGNAL(valueChanged(int)), pThermometer, SLOT(setValue(int)));

    grid->addWidget(pThermometer, 0, 1, Qt::AlignHCenter);
    boxgrid->addWidget(pSlider, 0, 0);

    pStartButton = new QPushButton(tr("Stop Automation"), group);
    pStartButton->setObjectName("ToggleTimer");
    pStartButton->setCheckable(true);
    pStartButton->setChecked(true);
    connect(pStartButton, SIGNAL(toggled(bool)), this, SLOT(toggleTimerControl1(bool)));

    boxgrid->addWidget(pStartButton, 2, 0);
    grid->addLayout(boxgrid, 0, 0, Qt::AlignCenter);

    pTimerStrumenti = new QTimer();
    connect(pTimerStrumenti, SIGNAL(timeout()), this, SLOT(slider1Update()));
    pTimerStrumenti->start(TIMEOUT);

    return page;
}
//------------------------------------------------------------------------------------------
//                              Amperemeter
//------------------------------------------------------------------------------------------
QWidget *mainForm::createAmperemeter(QWidget * pParent)
{
    QWidget *page = new QWidget(pParent);
    QVBoxLayout *toplay = new QVBoxLayout(page);

    QWidget *group = new QGroupBox(tr("Amperemeter"), page);
    QGridLayout *grid = new QGridLayout(group);
    QGridLayout *boxgrid = new QGridLayout(group);

    toplay->addWidget(group);

    // Amperemeter
    pAmperemeter = new QtSvgDialGauge(group);
    pAmperemeter->setSkin("Amperemeter");
    pAmperemeter->setNeedleOrigin(0.5, .83596590);
    pAmperemeter->setMinimum(0);
    pAmperemeter->setMaximum(360);
    pAmperemeter->setStartAngle(-42);
    pAmperemeter->setEndAngle(42);
    pAmperemeter->setValue(0);
    pAmperemeter->setMaximumSize(500,300);
    pAmperemeter->setMinimumSize(500,300);

    pSlider = new QSlider(Qt::Horizontal, group);
    pSlider->setRange(0, 360);
    pSlider->setValue(0);
    pSlider->setProperty("isEmbedded", m_embedded);
    pSlider->setMinimumSize(250,50);
    connect(pSlider, SIGNAL(valueChanged(int)), pAmperemeter, SLOT(setValue(int)));

    grid->addWidget(pAmperemeter, 0, 1, Qt::AlignHCenter);
    boxgrid->addWidget(pSlider, 0, 0);

    pStartButton = new QPushButton(tr("Stop Automation"), group);
    pStartButton->setObjectName("ToggleTimer");
    pStartButton->setCheckable(true);
    pStartButton->setChecked(true);
    connect(pStartButton, SIGNAL(toggled(bool)), this, SLOT(toggleTimerControl1(bool)));

    boxgrid->addWidget(pStartButton, 2, 0);
    grid->addLayout(boxgrid, 0, 0, Qt::AlignCenter);

    pTimerStrumenti = new QTimer();
    connect(pTimerStrumenti, SIGNAL(timeout()), this, SLOT(slider1Update()));
    pTimerStrumenti->start(TIMEOUT);

    return page;
}

//------------------------------------------------------------------------------------------
//                                      Graphs
//------------------------------------------------------------------------------------------
QWidget *mainForm::createGraphs(QWidget * pParent)
{

    QWidget *page       = new QWidget(pParent);
    QVBoxLayout *toplay = new QVBoxLayout(page);

    QWidget *group      = new QGroupBox(tr("Graph with Static or Dynamic Content"), page);
    QGridLayout *boxgrid = new QGridLayout(group);

    toplay->addWidget(group);

    pStaticGraph = new QtBasicGraph(group);
    pStaticGraph->setFocusPolicy(Qt::NoFocus);
    pStaticGraph->show();
    pStaticGraph->setMinimumSize(750,300);

    boxgrid->addWidget(pStaticGraph, 0, 0, 1, 4);

    QLabel *label = new QLabel(tr("Choose Graph Type"), group);
    boxgrid->addWidget(label, 1, 1, Qt::AlignRight);

    QComboBox *chooser = new QComboBox(group);
    chooser->addItems(QStringList() << tr("Static Graph") << tr("Dynamic Sinus Graph") << tr("Dynamic Heartbeat Graph"));
    connect(chooser, SIGNAL(currentIndexChanged(int)), this, SLOT(showGraph(int)));
    boxgrid->addWidget(chooser, 1, 2);

    pGraphTimer = new QTimer(this);
    connect(pGraphTimer, SIGNAL(timeout()), this, SLOT(generatorTickSlot()));

    showGraph(2);

    return page;
}
//---------------------------------------------------------------------------------------------
QWidget *mainForm::generatorTickSlot() {

    ++m_graph_tick;

    if (m_graph_type == 1) { // sinus
        double calc_at = (m_graph_tick % 360) * 3.1415926535798932846 / 180.0;
        pStaticGraph->addPoint(QPointF(m_graph_tick, sin(calc_at)));
    } else if (m_graph_type == 2) { // heartbeat
        int frame = m_graph_tick % 27;

        if (frame == 25)
            pStaticGraph->addPoint(QPointF(m_graph_tick, 5.0));
        else if (frame == 26)
            pStaticGraph->addPoint(QPointF(m_graph_tick, -2.0));
        else
            pStaticGraph->addPoint(QPointF(m_graph_tick, 0.5*qrand() / static_cast<double>(RAND_MAX)));
    } else {
        pGraphTimer->stop();
    }
}
//------------------------------------------------------------------------------------------
QWidget *mainForm::showGraph(int index)
{
    pGraphTimer->stop();
    m_graph_type = index;
    m_graph_tick = 0;

    if (index == 0) { // static

        pStaticGraph->clear();
        pStaticGraph->setYMinMax(-15, 15);
        pStaticGraph->setXRange(400);

        fillStaticGraph(pStaticGraph);

    } else
        if (index == 1) { // sinus

            pGraphTimer->start(33);

            pStaticGraph->clear();
            pStaticGraph->setYMinMax(-1.2, 1.2);
            pStaticGraph->setXRange(360);

    } else if (index == 2) { // heartbeat

        pGraphTimer->start(33);

        pStaticGraph->clear();
        pStaticGraph->setYMinMax(-10, 10);
        pStaticGraph->setXRange(180);
    }
}
//--------------------------------------------------------------------------------------------
void mainForm::fillStaticGraph(QtBasicGraph *graph)
{
    graph->addPoint(QPointF(0, 0));
    graph->addPoint(QPointF(20, 0));
    graph->addPoint(QPointF(20, 10));
    graph->addPoint(QPointF(50, 10));
    graph->addPoint(QPointF(50, -10));
    graph->addPoint(QPointF(80, -10));
    graph->addPoint(QPointF(80, 10));
    graph->addPoint(QPointF(110, 10));
    graph->addPoint(QPointF(110, -10));
    graph->addPoint(QPointF(140, -10));
    graph->addPoint(QPointF(140, 0));

    for (int i = 0; i < 120; ++i) {
        double val = sin(i / 120.0 * 3.1415926535 * 7.0);
        graph->addPoint(QPointF(140 + i, val * 5.0));
    }

    graph->addPoint(QPointF(260, 0));
    graph->addPoint(QPointF(260, -10));
    graph->addPoint(QPointF(290, -10));
    graph->addPoint(QPointF(290, 10));
    graph->addPoint(QPointF(320, 10));
    graph->addPoint(QPointF(320, -10));
    graph->addPoint(QPointF(350, -10));
    graph->addPoint(QPointF(350, 10));
    graph->addPoint(QPointF(380, 10));
    graph->addPoint(QPointF(380, 0));
    graph->addPoint(QPointF(400, 0));
}

//------------------------------------------------------------------------------------------
//                                      PatientCare
//------------------------------------------------------------------------------------------
QWidget *mainForm::createPatientCare(QWidget * pParent){

    QWidget *page = new QWidget(pParent);
    uiPatentCare.setupUi(page);
    setSkins();
    m_controller = new PatientCareController(&uiPatentCare, pParent);

    return(page);
}
//------------------------------------------------------------------------------------------
void mainForm::setSkins()
{
    uiPatentCare.switchSystemOff->setSkin("Beryl");
    uiPatentCare.switchNightMode->setSkin("Beryl");

    uiPatentCare.multiSliderMedicine->setSkin("Beryl");

    uiPatentCare.heartRateNavigator->setSkin("Beryl");
    uiPatentCare.multiSliderHeartRate->setSkin("Beryl");
    uiPatentCare.medicineFlow->setSkin("Beryl");
}
//------------------------------------------------------------------------------------------
//                                  BUTTONS
//------------------------------------------------------------------------------------------
QWidget *mainForm::createButtons(QWidget * pParent)
{
    QWidget *page = new QWidget(pParent);
    QGridLayout *toplay = new QGridLayout(page);

    QGroupBox *beryl    = new QGroupBox(tr("Beryl"), page);
    QGroupBox *berylsq  = new QGroupBox(tr("Beryl Square"), page);
    QGroupBox *metallic = new QGroupBox(tr("Metallic Brush"), page);
    metallic->setProperty("isMetallic", true);

    toplay->addWidget(beryl, 0, 0);
    toplay->addWidget(metallic, 1, 0);
    toplay->addWidget(berylsq, 0, 1, 2, 1);
    toplay->setColumnStretch(0, 60);
    toplay->setColumnStretch(1, 40);

    QGridLayout *bgrid = new QGridLayout(beryl);
    QGridLayout *mgrid = new QGridLayout(metallic);
    QGridLayout *sgrid = new QGridLayout(berylsq);


    QtSvgSlideSwitch* slide;
    QtSvgButton *button;
    QtSvgToggleSwitch *toggle;
    QLabel* label;

    // Beryl
    slide = new QtSvgSlideSwitch(beryl);
    slide->setSkin("Beryl");
    label = new QLabel(tr("QtSvgSlideSwitch"), beryl);
    label->setAlignment(Qt::AlignHCenter);
    bgrid->addWidget(slide, 0, 0, Qt::AlignHCenter);
    bgrid->addWidget(label, 1, 0, Qt::AlignHCenter);

    button = new QtSvgButton(beryl);
    button->setSkin("Beryl");
    label = new QLabel(tr("QtSvgButton"), beryl);
    label->setAlignment(Qt::AlignHCenter);
    bgrid->addWidget(button, 0, 1, Qt::AlignHCenter);
    bgrid->addWidget(label, 1, 1, Qt::AlignHCenter);

    toggle = new QtSvgToggleSwitch(beryl);
    toggle->setSkin("Beryl");
    label = new QLabel(tr("QtSvgToggleSwitch"), beryl);
    label->setAlignment(Qt::AlignHCenter);
    bgrid->addWidget(toggle, 0, 3, Qt::AlignHCenter);
    bgrid->addWidget(label, 1, 3, Qt::AlignHCenter);

    bgrid->setRowStretch(bgrid->rowCount(), 100);

    // Metallic Brush
    slide = new QtSvgSlideSwitch(metallic);
    slide->setSkin("MetallicBrush");
    label = new QLabel(tr("QtSvgSlideSwitch"), metallic);
    label->setAlignment(Qt::AlignHCenter);
    mgrid->addWidget(slide, 0, 0, Qt::AlignHCenter);
    mgrid->addWidget(label, 1, 0, Qt::AlignHCenter);

    button = new QtSvgButton(metallic);
    button->setSkin("MetallicBrush");
    label = new QLabel(tr("QtSvgButton"), metallic);
    label->setAlignment(Qt::AlignHCenter);
    mgrid->addWidget(button, 0, 1, Qt::AlignHCenter);
    mgrid->addWidget(label, 1, 1, Qt::AlignHCenter);

    mgrid->setRowStretch(mgrid->rowCount(), 100);

    // Beryl Square
    button = new QtSvgButton(beryl);
    button->setSkin("BerylSquare");
    label = new QLabel(tr("QtSvgButton"), berylsq);
    label->setAlignment(Qt::AlignHCenter);
    sgrid->addWidget(button, 0, 0, Qt::AlignHCenter);
    sgrid->addWidget(label, 1, 0, Qt::AlignHCenter);

    sgrid->setRowMinimumHeight(2, 16);

    toggle = new QtSvgToggleSwitch(metallic);
    toggle->setSkin("BerylSquare");
    label = new QLabel(tr("QtSvgToggleSwitch"), berylsq);
    label->setAlignment(Qt::AlignHCenter);
    sgrid->addWidget(toggle, 3, 0, Qt::AlignHCenter);
    sgrid->addWidget(label, 4, 0, Qt::AlignHCenter);

    sgrid->setRowStretch(sgrid->rowCount(), 100);


    return page;
}

//------------------------------------------------------------------------------------------
//                                  Advanced
//------------------------------------------------------------------------------------------
QWidget *mainForm::createSliders(QWidget * pParent)
{
    QWidget *page = new QWidget(pParent);
    QVBoxLayout *toplay = new QVBoxLayout(page);
    QWidget *group = new QGroupBox(tr("Beryl"), page);

    QGridLayout *grid = new QGridLayout(group);

    toplay->addWidget(group);

    QLabel *label;

    QtScrollWheel *scrollWheel = new QtScrollWheel(group);
    scrollWheel->setRange(0, 100);
    scrollWheel->setValue(50);
    scrollWheel->setSkin("Beryl");
    label = new QLabel(tr("QtScrollWheel"), group);
    label->setAlignment(Qt::AlignHCenter);
    grid->addWidget(scrollWheel, 0, 0, Qt::AlignHCenter);
    grid->addWidget(label,       1, 0, Qt::AlignHCenter);

    QtMultiSlider *multiSlider = new QtMultiSlider(group);
    multiSlider->setSkin("Beryl");
    multiSlider->setValue(70);
    label = new QLabel(tr("QtMultiSlider"), group);
    label->setAlignment(Qt::AlignHCenter);

    grid->addWidget(multiSlider, 0, 1, Qt::AlignHCenter);
    grid->addWidget(label,       1, 1, Qt::AlignHCenter);

    QSlider* slider = new QSlider(Qt::Vertical, group);
    slider->setValue(30);
    QString css = "QSlider::groove:vertical {"
                  "  border: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                  "    stop:0 #B1B1B1, stop:1 #000000);"
                  "}"
                  "QSlider::handle:vertical {"
                  "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                  "    stop:0 #DFDFDF, stop: 0.5 #7E7E7E, stop:1 #BCBCBC);"
                  "  border: 2px solid #709E10;"
                  "  border-radius: 3px;"
                  "  width: 30px;"
                  "  height: 30px;"
                  "} "
                  "QSlider::add-page:vertical {"
                  "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
                  "    stop: 0 #C3E0CC, stop: 0.5 #7FB605, stop:1 #D8F000);"
                  "}"
                  "QSlider::sub-page:vertical {"
                  "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
                  "    stop: 0 #CCD5CC, stop: 0.5 #435A12, stop:1 #5C6700);"
                  "}"
                  "QSlider::handle:pressed {"
                  "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                  "    stop:0 #DFDFDF, stop: 0.5 #7E7E7E, stop:1 #BCBCBC);"
                  "  border: 2px solid #D0F201;"
                  "  border-radius: 3px;"
                  "}";
    slider->setStyleSheet(css);
    label = new QLabel(tr("QSlider\n(Qt Style Sheets)"), group);
    label->setAlignment(Qt::AlignHCenter);

    grid->addWidget(slider, 0, 2, Qt::AlignHCenter);
    grid->addWidget(label,  1, 2, Qt::AlignHCenter);

    grid->setRowStretch(grid->rowCount(), 100);

    return page;
}
//------------------------------------------------------------------------------------------
//                             Table
//------------------------------------------------------------------------------------------
QWidget *mainForm::createMyTable(QWidget * pParent){

    QWidget *page = new QWidget(pParent);

    cGraphTable* pTable = new  cGraphTable(page);

    pTable->setGeometry(5,5,790,410);

    pTable->setNumPoint(16,0,16);

    int High = 16;
    int Low = 0;

    for(int i=0;i<16;i++){
        pTable->setValue(i,(qrand() % ((High + 1) - Low) + Low));
    }

    pTable->show();

    return(page);
}
//------------------------------------------------------------------------------------------
//                            Kinetic Scrolling
//------------------------------------------------------------------------------------------
QWidget *mainForm::createKineticList(QWidget * pParent){

    QWidget *page       = new QWidget(pParent);
    QVBoxLayout *toplay = new QVBoxLayout(page);
    QWidget *group      = new QGroupBox(tr("Kinetic List"), page);

    QGridLayout *grid   = new QGridLayout(group);

    toplay->addWidget(group);

    group->setMinimumSize(780,400);

    ColorList* pList = new ColorList(group);
    pList->resize(360, 500);
    pList->setGeometry(120,15,390,380);
    pList->show();

    return(page);
}

//------------------------------------------------------------------------------------------
//                            QSlide
//------------------------------------------------------------------------------------------
QWidget *mainForm::createSlidePage(QWidget * pParent){

    QWidget *page = new QWidget(pParent);
    QVBoxLayout *toplay = new QVBoxLayout(page);
    QWidget *group = new QGroupBox(tr("Test Gui"), page);

    QVBoxLayout *grid = new QVBoxLayout(group);

    toplay->addWidget(group);
    group->setMinimumSize(780,400);

    pSlide = new QSlide(group);

    pSlide->setGeometry(40,30,720,100);

    pSlide->setText("BALTO DEMO TEST GUI ");
    pSlide->show();
    pSlide->setTimer(500);
    pSlide->startTmr(true);
    pSlide->setMinimumSize(710,80);
    pSlide->setMaximumSize(710,80);
    effect = pSlide->effect();

    QPushButton* pBtnEff = new QPushButton(group);
    connect(pBtnEff,SIGNAL(clicked()),this,SLOT(changeEffectsSlot()));
    pBtnEff->setText("Set Effect");
    pBtnEff->setMinimumSize(120,60);
    pBtnEff->setMaximumSize(120,60);
    pBtnEff->setCheckable(false);
    pBtnEff->setFocusPolicy(Qt::NoFocus);

    QComboBox* pComboColor = new QComboBox(group);
    pComboColor->addItem("Green");
    pComboColor->addItem("Yellow");
    pComboColor->addItem("Red");

    connect(pComboColor,SIGNAL(currentIndexChanged(int)),this,SLOT(changeColorSlot(int)));

    pComboColor->setMinimumSize(300,60);
    pComboColor->setMaximumSize(300,60);

    color = 0;

    grid->addWidget(pSlide);
    grid->addWidget(pBtnEff);
    grid->addWidget(pComboColor);

    grid->setAlignment(pSlide,      Qt::AlignHCenter);
    grid->setAlignment(pBtnEff,     Qt::AlignHCenter);
    grid->setAlignment(pComboColor, Qt::AlignHCenter);

    //grid->setRowStretch(grid->rowCount(), 100);

    return(page);
}
//------------------------------------------------------------------------------------------
void mainForm::changeColorSlot(int val){

    color = val;
    if(color == 0)
        pSlide->setColorTx(Qt::green);
    else
         if(color == 1)
        pSlide->setColorTx(Qt::yellow);
        else
             pSlide->setColorTx(Qt::red);

}
//------------------------------------------------------------------------------------------
void mainForm::changeEffectsSlot(){

    effect = !effect;

    if(effect == 0)
        pSlide->setEffect(QSlide::sliding);
    else
        pSlide->setEffect(QSlide::intermittent);
}

//------------------------------------------------------------------------------------------
//                            Widget Page
//------------------------------------------------------------------------------------------
QWidget *mainForm::createWidgetShowPage(QWidget * pParent){


}
//------------------------------------------------------------------------------------------
//                            TRIVAL SLIDE SHOW IMAGE
//------------------------------------------------------------------------------------------
QWidget *mainForm::createTrivalImageShowPage(QWidget * pParent){

    QWidget *page = new QWidget(pParent);
    page->setGeometry(0,0,pParent->width(), pParent->height());
    pTrivalImage = new MyPictureSlideShow(page);
    pTrivalImage->setGeometry(0,0,page->width(), page->height());
    pTrivalImage->show();
    return(page);
}
//------------------------------------------------------------------------------------------
void mainForm::timerEvent(QTimerEvent * ev){

    if( ev->timerId() == checkMemIdx){

        size_t _used_memory = pMyAllocator->getUsedMemory();

        QString str = QString("Heap Usato:%1 %2Kb %3Mb - n°Allocazioni=%4 MaxMemoriaUsata:%5Kb").arg(_used_memory).arg(_used_memory/1024).arg(_used_memory/0x100000).arg(pMyAllocator->getNumAllocations()).arg(pMyAllocator->getMaxUsedSize()/1024);

        this->setWindowTitle ( str );
    }
}
