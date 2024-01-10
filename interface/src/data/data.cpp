/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-08 14:36:18
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-08 15:16:26
 * @FilePath: \EOS\interface\src\data\data.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "data.h"

DataWidget::DataWidget(QWidget*parent):QWidget(parent)
{
    initObjects();
    initAttributes();
    initLayout();
    initConnections();
}

void DataWidget::toggleCanvas(int idx)
{
    Q_UNUSED(idx);
    auto w = sender();
}

void DataWidget::initLayout()
{
    // 1. 左侧布局
    
    leftSplitter->addWidget(stackCanvas);
    leftSplitter->addWidget(gridPictureCanvas);
    leftSplitter->setStretchFactor(0,1);
    leftSplitter->setStretchFactor(1,4);

    // 2. 右侧布局
    auto grouplay = new QVBoxLayout;
    grouplay->addWidget(experTable);
    rightBox->setLayout(grouplay);

    // 3. 总布局
    auto lay = new QHBoxLayout;
    auto splitter = new Splitter(Qt::Horizontal);
    splitter->addWidget(leftSplitter);
    splitter->addWidget(rightBox);
    lay->addWidget(splitter);
    setLayout(lay);
}

void DataWidget::initConnections()
{
    connect(stackCanvas,&QStackedWidget::currentChanged,this,&DataWidget::toggleCanvas);
}

void DataWidget::initAttributes()
{
    stackCanvas->addWidget(pictureCanvas);
    pictureCanvas->setImage(CURRENT_PATH+"/images/cell.png");
}

void DataWidget::initObjects()
{
    stackCanvas = new QStackedWidget;
    pictureCanvas = new SinglePictureCanvas;
    gridPictureCanvas = new GridPictureCanvas;
    leftSplitter = new Splitter(Qt::Vertical);
    rightBox = new GroupBox;
    experTable = new ExperTable;
}

void DataWidget::resizeEvent(QResizeEvent *event)
{
    gridPictureCanvas->setMaximumHeight(event->size().height()/4);
    rightBox->setFixedWidth(event->size().width()/3);
    event->accept();
}