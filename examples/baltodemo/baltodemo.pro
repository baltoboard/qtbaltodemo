TEMPLATE = app
INCLUDEPATH += .

include(../../src/common/common.pri)
include(../../src/5waybutton/5waybutton.pri)
include(../../src/basicgraph/basicgraph.pri)
include(../../src/multislider/multislider.pri)
include(../../src/scrolldial/scrolldial.pri)
# omit scrollwheel, it's already included in scrolldial.pri
include(../../src/svgbutton/svgbutton.pri)
include(../../src/svgdialgauge/svgdialgauge.pri)
include(../../src/svgslideswitch/svgslideswitch.pri)
include(../../src/svgtoggleswitch/svgtoggleswitch.pri)

CONFIG += release
CONFIG -= debug

DEFINES  += _demoBalto


# Input
HEADERS += \
    mainform.h \
    patientcarecontroller.h \
    datagenerator.h \
    cgraphtable.h \
    flickable.h \
    qslide.h \
    allocator.h \
    lazytimer.h \
    mypictureslideshow.h


SOURCES += \
    main.cpp \
    mainform.cpp \
    patientcarecontroller.cpp \
    datagenerator.cpp \
    cgraphtable.cpp \
    flickable.cpp \
    qslide.cpp \
    allocator.cpp \
    lazytimer.cpp \
    mypictureslideshow.cpp


RESOURCES += \
    ../../skins/thermometer_svgdialgauge.qrc \
    ../../skins/tachometer_svgdialgauge.qrc \
    ../../skins/amperemeter_svgdialgauge.qrc \
    ../../skins/beryl_5waybutton.qrc \
    ../../skins/beryl_multislider.qrc \
    ../../skins/beryl_svgslideswitch.qrc \
    ../../skins/beryl_svgbutton.qrc \
    ../../skins/beryl_svgtoggleswitch.qrc \
    ../../skins/berylsquare_svgtoggleswitch.qrc \
    ../../skins/berylsquare_svgbutton.qrc \
    ../../skins/beryl_scrollwheel.qrc \
    ../../skins/beryl_scrolldial.qrc \
    ../../skins/metallicbrush_svgbutton.qrc \
    ../../skins/metallicbrush_svgslideswitch.qrc \
    patientcare.qrc \
    resources.qrc

RESOURCES += baltodemo.qrc

QT *= svg

FORMS += \
    mainform.ui \
    patientcare.ui
