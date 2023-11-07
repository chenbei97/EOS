/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 11:43:49
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 11:46:38
 * @FilePath: \EOS\interface\src\window\mainwindow.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    navigbar = new NavigBar;
    appselect = new AppSelect;
    preview = new Preview;
    setting = new Setting;
    stack = new QStackedWidget;

    navigbar->setMinimumHeight(NavigBarHeight);

    stack->addWidget(appselect);
    stack->addWidget(preview);
    stack->addWidget(new QWidget);
    stack->addWidget(new QWidget);
    stack->addWidget(setting);

    auto w = new QWidget;
    auto s = new QVBoxLayout;
    s->addWidget(navigbar);
    s->addWidget(stack);
    s->setMargin(0);
    s->setSpacing(0);
    w->setLayout(s);
    setCentralWidget(w);

    //setAttribute(Qt::WA_DeleteOnClose,true);
    resize(MainWindowWidth,MainWindowHeight);

    connect(navigbar,&NavigBar::buttonClicked,this,&MainWindow::navigbarSelect);
    connect(appselect,&AppSelect::proliferationClicked,this,[=]{stack->setCurrentIndex(1);});
    connect(appselect,&AppSelect::scratchClicked,this,[=]{stack->setCurrentIndex(1);});
    connect(appselect, QOverload<int>::of(&AppSelect::appClicked),preview,&Preview::setAppInfo);

    connect(setting,&Setting::objectiveSettingChanged,preview,&Preview::objectiveSettingChanged);
    setting->emitSignals(); // 把objectivesetting的初始配置触发一下去更新previewtool


    // start("Eos_I","Eos_main","main");
    StartPythonPointer->start("../test","test_socket","test_server");
}

void MainWindow::navigbarSelect(int id)
{
    stack->setCurrentIndex(id);
//#ifdef notusetoupcamera
//    if (id == 1) preview->openCamera();
//        else  preview->closeCamera();
//#else
//    if (id == 1) ToupCameraPointer->openCamera();
//    else  ToupCameraPointer->closeCamera();
//#endif
}