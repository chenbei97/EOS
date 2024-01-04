/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 11:43:49
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 11:46:38
 * @FilePath: \EOS\interface\src\window\mainwindow.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "mainwindow.h"
#include "dataWidget.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    navigbar = new NavigBar;
    appselect = new AppSelect;
    preview = new Preview;
    setting = new Setting;
    stack = new QStackedWidget;
    // 加载数据页面
    dataWidget dataW;
    mDataWidget = dataW.ShowView();
    navigbar->setMinimumHeight(NavigBarHeight);

    stack->addWidget(appselect);
    stack->addWidget(preview);
    //stack->addWidget(new QWidget);
    stack->addWidget(mDataWidget);
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
    quitfullscreen = new QAction(tr("quit full screen"));
    enterfullscreen = new QAction(tr("enter full screen"));
    addAction(enterfullscreen);
    addAction(quitfullscreen);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(navigbar,&NavigBar::buttonClicked,this,&MainWindow::navigbarSelect);
    connect(appselect,&AppSelect::proliferationClicked,this,[=]{stack->setCurrentIndex(1);});
    connect(appselect,&AppSelect::scratchClicked,this,[=]{stack->setCurrentIndex(1);});
    connect(appselect, QOverload<int>::of(&AppSelect::appClicked),preview,&Preview::setAppInfo);
    connect(quitfullscreen,&QAction::triggered,[=]{showMaximized();});
    connect(enterfullscreen,&QAction::triggered,[=]{showFullScreen();});

    connect(setting,&Setting::objectiveSettingChanged,preview,&Preview::objectiveSettingChanged);
    //setting->emitSignals(); // 把objectivesetting的初始配置触发一下去更新previewtool(如果一开始就对应上也可以不手动触发)
}

void MainWindow::askConnectState()
{
    preview->askConnectState();
}

void MainWindow::askActivateCode()
{
    preview->askActivateCode();
}

void MainWindow::navigbarSelect(int id)
{
    stack->setCurrentIndex(id);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qApp->quit();
    event->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        if (isFullScreen()) {
            hide();
            showMaximized();
        }
    }
    event->accept();
}