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

    resize(MainWindowWidth,MainWindowHeight);

    connect(navigbar,&NavigBar::buttonClicked,stack,&QStackedWidget::setCurrentIndex);

    connect(appselect,&AppSelect::proliferationClicked,this,[=]{stack->setCurrentIndex(1);});
    connect(appselect,&AppSelect::scratchClicked,this,[=]{stack->setCurrentIndex(1);});
    connect(appselect, QOverload<int>::of(&AppSelect::appClicked),preview,&Preview::setAppInfo);

    connect(qApp,&QApplication::aboutToQuit, &StartPython::instance(),&StartPython::quit);

    connect(setting,&Setting::objectiveSettingChanged,preview,&Preview::objectiveSettingChanged);
    setting->emitSignals(); // 把objectivesetting的初始配置触发一下去更新previewtool

//    ToupcamDeviceV2 m_cur;
//    ToupcamDeviceV2 arr[TOUPCAM_MAX] = {0};
//    unsigned count = Toupcam_EnumV2(arr);
//    if (0 == count)
//        QMessageBox::warning(nullptr, "Warning", "No camera found.");
    //Py_Initialize();

    //StartPythonPointer->start("../test","test_socket","test_server"); // start("Eos_I","Eos_main","main");
    StartPython::instance().start("../test","test_socket","test_server");
}

MainWindow::~MainWindow() noexcept
{
    //QTimer::singleShot(0,qApp,SLOT(quit()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qApp->quit();
    QMainWindow::closeEvent(event);
}