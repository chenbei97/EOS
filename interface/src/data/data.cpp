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
        {
            for(int i = 0; i < gridStack->count(); ++i)
            {
                auto w = static_cast<GridPictureCanvas*>(gridStack->widget(i));
                if (w->isRunning()) {
                    w->appendImage(img_path);
                }
            }
        }
    }
}

void DataWidget::toggleExperiment(int row, bool isRunning,const PlateImageInfo& info)
{ // 切换实验
    gridFilterStack->setCurrentWidget(gridStack);
    gridStack->setCurrentIndex(row);
    dataPatternStack->setCurrentIndex(row);
    patternTableStack->setCurrentIndex(row);
    timeTableStack->setCurrentIndex(row);
    static_cast<DataPattern*>(dataPatternStack->currentWidget())->initHoleInfo(info); // 点击时才更新
    auto w = static_cast<GridPictureCanvas*>(gridStack->currentWidget());
    Q_ASSERT(isRunning == w->isRunning());
    if (!isRunning) { // 如果是完成的实验直接显示某个文件夹下的所有图片应该
        w->showImages(CURRENT_PATH+"/images/images",DataReadImageFormatFilter); // 注意过滤后缀
    } else { // 如果切到running实验,是根据异步信号不断更新的

    }

}

void DataWidget::addStackWidget(const PlateImageInfo& info)
{
    auto isRunning = info.isRunning;
    auto pattern_w = info.plateSize.width();
    auto pattern_h = info.plateSize.height();

    // 1. 创建该实验的图像展示画布
    auto gridcanvas = new GridPictureCanvas;
    gridcanvas->setState(isRunning);

    gridStack->addWidget(gridcanvas); // 每个实验都有自己的画布用于running时

    // 2. 创建该实验对应的图案
    auto pattern = new DataPattern(pattern_w,pattern_h);
    dataPatternStack->addWidget(pattern);

    // 3. 创建该实验对应的2个表
    auto patternTable = new PatternTable;

    // 点孔切到筛选图片的堆叠面板
    patternTableStack->addWidget(patternTable);

    auto timeTable = new TimeTable;
    // 点左表切到筛选图片的堆叠面板
    timeTableStack->addWidget(timeTable);

    //pattern->initHoleInfo(info); // 创建时不要更新,toggleExperiment切换才去更新
    connect(gridcanvas,&GridPictureCanvas::imageClicked,pictureCanvas,QOverload<const QImage&,const QString&>::of(&SinglePictureCanvas::setImage));
    connect(pattern,QOverload<const DataPatternHoleInfo&>::of(&DataPattern::holeClicked),patternTable,&PatternTable::refreshTable);
    connect(pattern,QOverload<const DataPatternHoleInfo&>::of(&DataPattern::holeClicked),[=]{gridFilterStack->setCurrentWidget(filterGridCanvas);});
    connect(patternTable,&PatternTable::currentHoleViewChannelInfo,timeTable,&TimeTable::refreshTable);
    connect(patternTable,&PatternTable::currentHoleViewChannelInfo,filterGridCanvas,&DataGrid::displayImages);
    connect(patternTable,&PatternTable::currentHoleViewChannelInfo,[=]{gridFilterStack->setCurrentWidget(filterGridCanvas);});
    connect(timeTable,&TimeTable::imageClicked,pictureCanvas,QOverload<const QString&>::of(&SinglePictureCanvas::setImage));
}

void DataWidget::initConnections()
{
    //connect(dataTable,&DataTable::currentRowChanged,this,&DataWidget::toggleExperiment);
    connect(dataTable,&DataTable::currentRowClicked,this,&DataWidget::toggleExperiment); // 改为点击行触发
    connect(dataTable,&DataTable::experRecordAppended,this,&DataWidget::addStackWidget);
    connect(ParserPointer,&ParserControl::parseResult,this,&DataWidget::parseResult);
    connect(filterGridCanvas,&DataGrid::imageClicked,pictureCanvas,QOverload<const QImage&,const QString&>::of(&SinglePictureCanvas::setImage));
}

void DataWidget::initAttributes()
{
    pictureStack->addWidget(pictureCanvas);
    pictureCanvas->setImage(CURRENT_PATH+"/images/images/1"+DataReadImageFormat);
    //pictureCanvas->setImage(CURRENT_PATH+"/images/direction.png");
    filterGridCanvas->setState(false);
    gridFilterStack->addWidget(gridStack);
    gridFilterStack->addWidget(filterGridCanvas);
}

void DataWidget::initLayout()
{
    // 1. 左侧布局
    auto leftlay = new QVBoxLayout;
    leftlay->addWidget(pictureStack);
    leftlay->addWidget(gridFilterStack);
    leftBox->setLayout(leftlay);

    // 2. 右侧布局
    auto rightlay = new QVBoxLayout;
    rightlay->addWidget(dataTable);
    rightlay->addWidget(dataPatternStack);
    auto tablelay = new QHBoxLayout;
    tablelay->addWidget(patternTableStack);
    tablelay->addWidget(timeTableStack);
    rightlay->addLayout(tablelay);
    rightlay->addWidget(dataImage);
    rightBox->setLayout(rightlay);

    // 3. 总布局
    auto lay = new QHBoxLayout;
    auto splitter = new Splitter(Qt::Horizontal);
    splitter->addWidget(leftBox);
    splitter->addWidget(rightBox);
    lay->addWidget(splitter);
    setLayout(lay);
}

void DataWidget::initObjects()
{
    leftBox = new GroupBox;
    rightBox = new GroupBox;
    pictureStack = new QStackedWidget;
    gridStack = new QStackedWidget;
    dataPatternStack = new QStackedWidget;
    patternTableStack = new QStackedWidget;
    timeTableStack = new QStackedWidget;

    filterGridCanvas = new DataGrid; // 共用
    gridFilterStack = new QStackedWidget;
    pictureCanvas = new SinglePictureCanvas;
    dataTable = new DataTable;
    dataImage = new DataImage;
}

void DataWidget::resizeEvent(QResizeEvent *event)
{
    gridFilterStack->setMaximumHeight(event->size().height()/4);
    rightBox->setFixedWidth(event->size().width()/12*5);

    dataTable->setFixedHeight(event->size().height()/4); // 实验表
    dataPatternStack->setFixedHeight(event->size().height()/4);//图案

    patternTableStack->setMinimumWidth(rightBox->width()/3);
    patternTableStack->setFixedHeight(event->size().height()/4); // 左表

    timeTableStack->setFixedHeight(event->size().height()/4); // 右表
    event->accept();
}

DataWidget::DataWidget(QWidget*parent):QWidget(parent)
{
    initObjects();
    initAttributes();
    initLayout();
    initConnections();
    dataTable->initTableFromDataBase();
}