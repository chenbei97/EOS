/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-02 14:01:20
 * @FilePath: \EOS\interface\src\preview\preview.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

Preview::Preview(QWidget*parent): QWidget(parent)
{
    initObjects();
    initAttributes();
    initConnections();
    initLayout();
}

void Preview::onManufacturerChanged(int option)
{

}

void Preview::onWellbrandChanged(int option)
{
    updateViewPatternUi();
    switch (option) {
        case 0: pattern->setPatternSize(2,3);
            break;
        case 1: pattern->setPatternSize(4,6);
            break;
        case 2: pattern->setPatternSize(8,12);
            break;
        case 3: pattern->setPatternSize(16,24);
            break;
    }

}

void Preview::onObjectiveChanged(const QString& obj)
{
    //LOG<<"objective option = "<<option;
    updateViewPatternUi();
}

void Preview::setAppInfo(int app)
{ // 用于appselect传递当前app信息
    previewinfo[AppSelectField] = QString::number(app);
}

void Preview::initLayout()
{
    // 1.右侧布局
    auto pbox = new GroupBox(tr("选孔"));
    auto play = new QVBoxLayout;
    play->addWidget(pattern);
    //play->addWidget(dockcanvas); // 不加了通过点击孔来触发
    pbox->setLayout(play);

    auto rlay = new QVBoxLayout;
    rlay->addWidget(pbox);
    rlay->addWidget(scrollarea); // 添加滚动区域后(不是添加toolbar)再设置
    scrollarea->setWidget(toolbar);
    auto rbox = new GroupBox;
    rbox->setLayout(rlay);
    rbox->setMaximumWidth(PreviewToolBarMaxWidth);

    // 2.左侧布局
    stack->addWidget(livecanvas);
    stack->addWidget(photocanvas);
    auto llay = new QVBoxLayout;
    llay->addWidget(cameramode);
    llay->addWidget(stack);
    auto lbox = new GroupBox;
    lbox->setLayout(llay);

    // 3.总布局
    auto lay = new QHBoxLayout;
    lay->addWidget(lbox);
    lay->addWidget(rbox);
    setLayout(lay);
}

void Preview::initAttributes()
{
    scrollarea->setWidgetResizable(true);

    dock->setWidget(viewpattern);
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    dockcanvas->setCentralWidget(dock);
    dockcanvas->addDockWidget(Qt::BottomDockWidgetArea,dock);// 这个地方不能选all/no,否则无法实现点击隐藏关闭效果

    pattern->setMinimumHeight(PreviewPatternMinHeight);

    livecanvas->setStrategy(PhotoCanvas::SinglePixmap);
    photocanvas->setStrategy(PhotoCanvas::SinglePixmap);

    //previewinfo[PreviewToolField] = toolbar->toolInfo();
    //previewinfo[PreviewPatternField] = pattern->patternInfo();
}

void Preview::initObjects()
{
    cameramode = new CameraMode;
    livecanvas = new PhotoCanvas;
    photocanvas = new PhotoCanvas;
    stack = new QStackedWidget;
    pattern = new WellPattern(2,3);
    groupinfo = new GroupInfo;
    toolbar = new PreviewTool;
    viewpattern = new ViewPattern;// 视野窗口
    dock = new DockWidget(tr("选择孔内视野"));
    dockcanvas = new QMainWindow;
    scrollarea = new QScrollArea;
    Toupcam_GigeEnable(nullptr, nullptr);// 初始化对相机的支持

}

void Preview::initConnections()
{
    connect(toolbar,&PreviewTool::manufacturerChanged,this,&Preview::onManufacturerChanged);
    connect(toolbar,&PreviewTool::wellbrandChanged,this,&Preview::onWellbrandChanged);
    connect(toolbar,&PreviewTool::objectiveChanged,this,&Preview::onObjectiveChanged);
    connect(toolbar,&PreviewTool::cameraAdjusted,this,&Preview::adjustCamera);
    connect(toolbar,&PreviewTool::photoTaking,this,&Preview::takingPhoto);
    connect(toolbar,&PreviewTool::directionMove,this,&Preview::adjustLens);
    connect(toolbar,&PreviewTool::channelChanged,this,&Preview::toggleChannel);
    connect(toolbar,&PreviewTool::exportFilePath,this,&Preview::saveExperConfig);
    connect(toolbar,&PreviewTool::loadExper,this,&Preview::loadExper);

    connect(ParserPointer,&ParserControl::parseResult,this,&Preview::onAdjustCamera);
#ifdef notusetoupcamera
    connect(this, &Preview::evtCallback, this, &Preview::processCallback);
#else
    connect(ToupCameraPointer,&ToupCamera::imageCaptured,this,&Preview::showCapturedImage);
#endif

    connect(cameramode,&CameraMode::cameraModeChanged,this,[=](int option){stack->setCurrentIndex(option);});

    connect(pattern,&WellPattern::openViewWindow,this,&Preview::updateViewWindow); // 打开和更新视野窗口
    connect(pattern,&WellPattern::openSetGroupWindow,this,&Preview::updateSetGroupWindow);// 打开分组窗口
    connect(pattern,&WellPattern::mouseClicked,this,&Preview::previewViewByClickHole); // 点击孔也触发预览
    connect(pattern,&WellPattern::clearViewWindowCache,viewpattern,&ViewPattern::clearViewWindowCache);

    connect(viewpattern,&ViewPattern::applyGroupEvent,pattern,&WellPattern::updateHoleInfoByViewInfoApplyGroup); // 按组更新孔窗口的信息
    connect(viewpattern,&ViewPattern::applyAllEvent,pattern,&WellPattern::updateHoleInfoByViewInfoApplyAll); // 不安组更新孔窗口的信息
    connect(viewpattern,&ViewPattern::previewEvent,this,&Preview::previewViewByClickView); // 点击视野预览

    connect(this,&Preview::objectiveSettingChanged,toolbar,&PreviewTool::objectiveSettingChanged);
}