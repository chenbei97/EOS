/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-08 14:36:18
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-08 15:16:26
 * @FilePath: \EOS\interface\src\data\data.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "data.h"

void DataWidget::parseResult(const QString & f,const QVariant & d)
{
    if (f == "test0x4") {
        auto img_path = d.toString();
        if (!img_path.isEmpty())
            gridRunningCanvas->appendImage(img_path);
    }
}

void DataWidget::toggleExperiment(int row, bool isRunning)
{ // 切换实验
    LOG<<row<<isRunning;
    isRunning?
    bottomCanvas->setCurrentWidget(gridRunningCanvas):
    bottomCanvas->setCurrentWidget(gridCompeleCanvas);
}

void DataWidget::toggleCanvas(int idx)
{
    Q_UNUSED(idx);
    auto w = sender();
}

void DataWidget::initConnections()
{
    connect(topCanvas,&QStackedWidget::currentChanged,this,&DataWidget::toggleCanvas);
    connect(dataTable,&DataTable::currentRowChanged,this,&DataWidget::toggleExperiment);
    connect(ParserPointer,&ParserControl::parseResult,this,&DataWidget::parseResult);
}

void DataWidget::initAttributes()
{
    topCanvas->addWidget(pictureCanvas);
    pictureCanvas->setImage(CURRENT_PATH+"/images/cell.png");

    bottomCanvas->addWidget(gridCompeleCanvas);
    bottomCanvas->addWidget(gridRunningCanvas);
    gridRunningCanvas->setState(true);
    gridCompeleCanvas->setState(false);
}

void DataWidget::initLayout()
{
    // 1. 左侧布局
    leftSplitter->addWidget(topCanvas);
    leftSplitter->addWidget(bottomCanvas);
    leftSplitter->setStretchFactor(0,1);
    leftSplitter->setStretchFactor(1,4);

    // 2. 右侧布局
    auto grouplay = new QVBoxLayout;
    grouplay->addWidget(dataTable);
    grouplay->addWidget(dataPattern);
    auto tablelay = new QHBoxLayout;
    tablelay->addWidget(patternTable);
    tablelay->addWidget(timeTable);
    grouplay->addLayout(tablelay);
    grouplay->addWidget(dataImage);
    rightBox->setLayout(grouplay);

    // 3. 总布局
    auto lay = new QHBoxLayout;
    auto splitter = new Splitter(Qt::Horizontal);
    splitter->addWidget(leftSplitter);
    splitter->addWidget(rightBox);
    lay->addWidget(splitter);
    setLayout(lay);
}


void DataWidget::initObjects()
{
    topCanvas = new QStackedWidget;
    pictureCanvas = new SinglePictureCanvas;

    bottomCanvas = new QStackedWidget;
    gridRunningCanvas = new GridPictureCanvas;
    gridCompeleCanvas = new GridPictureCanvas;

    leftSplitter = new Splitter(Qt::Vertical);
    rightBox = new GroupBox;
    dataTable = new DataTable;
    dataPattern = new DataPattern(8,12);
    patternTable = new PatternTable;
    timeTable = new TimeTable;
    dataImage = new DataImage;
}

void DataWidget::resizeEvent(QResizeEvent *event)
{
    bottomCanvas->setMaximumHeight(event->size().height()/4);
    rightBox->setFixedWidth(event->size().width()/12*5);
    dataTable->setFixedHeight(event->size().height()/4);
    dataPattern->setFixedHeight(event->size().height()/4);
    patternTable->setFixedHeight(event->size().height()/4);
    timeTable->setFixedHeight(event->size().height()/4);
    patternTable->setMinimumWidth(rightBox->width()/3*2);
    event->accept();
}

DataWidget::DataWidget(QWidget*parent):QWidget(parent)
{
    initObjects();
    initAttributes();
    initLayout();
    initConnections();
}