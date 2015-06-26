/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/
#include "allocator.h"
#include <QApplication>

#include "mainform.h"

bool gCloseAndReOpen = false;

#ifdef _demoBalto
    bool gShowWheel = false;
#endif

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    int gDemoNum = 0;
    bool noSkin   = false;
    bool noMemory = false;


    QStringList args = QCoreApplication::arguments();
    if(args.count()){

        for(int i=1;i<args.count();i++){
            if(args.at(i).contains("-ndemo")){
                if( (i+1) < args.count()){
                    gDemoNum = args.at(i+1).toInt();
                }
            }else{
                if(args.at(i).contains("-noskin")){
                    noSkin = true;
                }else{
                    if(args.at(i).contains("-checkmemory")){
                        noMemory = true;
                    }else{
                        if(args.at(i).contains("-reopen")){
                            gCloseAndReOpen = true;
                        }
#ifdef _demoBalto
                        else{
                            if(args.at(i).contains("-wheel")){
                                gShowWheel = true;
                            }
                        }
#endif
                    }
                }
            }
        }
    }

    if (noMemory == true){
        pMyAllocator = new Allocator();
    }


    if(noSkin == false){
        QFile f(":/baltodemo.css");
        if (f.open(QIODevice::ReadOnly)) {
            app.setStyleSheet(f.readAll());
            f.close();
        }
    }

    mainForm* pMainForm = new mainForm(gDemoNum);

    //pMainForm->setAttribute( Qt :: WA_PaintOnScreen,true);

    pMainForm->show();

    if(pMyAllocator != NULL){
        pMyAllocator->printMem();
    }

    return app.exec();
}
