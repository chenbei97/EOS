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
    navigbar->setMinimumHeight(NavigBarHeight);

    appselect = new AppSelect;
    preview = new Preview;

    auto stacklay = new QStackedLayout;
    stacklay->addWidget(appselect);
    stacklay->addWidget(preview);

    auto w = new QWidget;
    auto s = new QVBoxLayout;
    s->addWidget(navigbar);
    s->addLayout(stacklay);
    s->setMargin(0);
    s->setSpacing(0);
    w->setLayout(s);

    setCentralWidget(w);
    resize(MainWindowWidth,MainWindowHeight);

    connect(navigbar,&NavigBar::mainClicked,this,[=]{stacklay->setCurrentIndex(0);});
    connect(navigbar,&NavigBar::previewClicked,this,[=]{stacklay->setCurrentIndex(1);});

    connect(appselect,&AppSelect::proliferationClicked,this,[=]{stacklay->setCurrentIndex(1);});
    connect(appselect,&AppSelect::scratchClicked,this,[=]{stacklay->setCurrentIndex(1);});
}