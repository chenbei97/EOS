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

    auto w = new QWidget;
    auto s = new QVBoxLayout;
    s->addWidget(navigbar);
    s->addWidget(preview);
    s->setMargin(0);
    s->setSpacing(0);
    w->setLayout(s);

    setCentralWidget(w);
    resize(MainWindowWidth,MainWindowHeight);
}