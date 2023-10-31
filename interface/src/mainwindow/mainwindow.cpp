/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 11:43:49
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 11:46:38
 * @FilePath: \EOS\interface\src\window\mainwindow.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "include/mainwindow/mainwindow.h"

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

    resize(MainWindowWidth,MainWindowHeight);

    connect(navigbar,&NavigBar::buttonClicked,stack,&QStackedWidget::setCurrentIndex);

    connect(appselect,&AppSelect::proliferationClicked,this,[=]{stack->setCurrentIndex(1);});
    connect(appselect,&AppSelect::scratchClicked,this,[=]{stack->setCurrentIndex(1);});
    connect(appselect, SIGNAL(appClicked(int)),this, SLOT(onAppSelected(int)));

    connect(setting,&Setting::locationChanged,preview,&Preview::onLocationSettingChanged);
    connect(setting,&Setting::objectiveChanged,preview,&Preview::objectiveSettingChanged);

    setting->locationChanged(0); // 触发时机只能在setting构造出来之后,内部触发不会起作用
    setting->objectiveChanged(0);
}


void MainWindow::onAppSelected(int app)
{ // 把选择的app信息传给预览界面
    QVariantMap m;
    m[AppSelectField] = QString::number(app);
    preview->setPreviewInfo(m);
}