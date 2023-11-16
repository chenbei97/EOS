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

    // 1.更新视野的尺寸
    auto toolinfo = previewtool->toolInfo();
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);

    // 2. 更新视野窗口去更新视野窗口绘制和临时保存信息
    //LOG<<manufacturer<<brand<<objective<<size;
    viewpattern->clearAllViewWindowCache(size);

    // 3. 视野窗口的数据信息临时信息需要更改,因为尺寸变了
    dock->setWindowTitle(tr("Select Hole Inside View"));

    // 4.切换厂家setPattenSize已经都清理完了,无需再调用
}

void Preview::onObjectiveChanged(const QString& obj)
{ // 切换物镜的尺寸,视野尺寸发生变化
    LOG<<"objective option = "<<obj;
    // 1.更新视野的尺寸
    auto toolinfo = previewtool->toolInfo();
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);

    // 2. 更新视野窗口去更新视野窗口绘制和临时保存信息
    auto current_size = viewpattern->currentViewInfo()[HoleViewSizeField].toSize().width();

    if (current_size != size) { // 如果视野尺寸前后没发生变化,不需要清理
        LOG<<"切换物镜前视野尺寸: "<<current_size<<" 切换物镜后尺寸: "<<size<<" 需要清理";
        viewpattern->clearAllViewWindowCache(size);
        // 3. 视野窗口的数据信息临时信息需要更改,因为尺寸变了
        dock->setWindowTitle(tr("Select Hole Inside View"));
        pattern->clearAllHoleViewPoints();// 只需要清理视野信息,其它保留
    } else {
        LOG<<"切换物镜前视野尺寸: "<<current_size<<" 切换物镜后尺寸: "<<size<<" 无需清理";
    }
}

void Preview::updateViewPatternUi()
{// 弃用,切换厂家和切换物镜分开写
    // 1.更新视野的尺寸
    auto toolinfo = previewtool->toolInfo();
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);

    // 2. 更新视野窗口去更新视野窗口绘制和临时保存信息
    auto current_size = viewpattern->currentViewInfo()[HoleViewSizeField].toSize().width();
    if (current_size != size) {
        LOG<<"切换物镜前视野尺寸: "<<current_size<<" 切换物镜后尺寸: "<<size<<" 需要清理";
        viewpattern->clearAllViewWindowCache(size);
        // 3. 视野窗口的数据信息临时信息需要更改,因为尺寸变了
        dock->setWindowTitle(tr("Select Hole Inside View"));
        pattern->clearAllHoleViewPoints();// 只需要清理视野信息,其它保留,切换厂家setPattenSize已经都清理完了
    } else {
        LOG<<"切换物镜前视野尺寸: "<<current_size<<" 切换物镜后尺寸: "<<size<<" 无需清理";
    }
}

void Preview::setAppInfo(int app)
{ // 用于appselect传递当前app信息
    previewinfo[AppSelectField] = QString::number(app);
}

void Preview::initLayout()
{
    // 1.右侧布局
    auto pbox = new GroupBox(tr("Hole Selection"));
    auto play = new QVBoxLayout;
    play->addWidget(pattern);
    pbox->setLayout(play);

    auto rlay = new QVBoxLayout;
    rlay->addWidget(pbox);
    rlay->addWidget(scrollarea); // 添加滚动区域后(不是添加previewtool)再设置
    scrollarea->setWidget(previewtool);

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
#ifdef usetab
    tab->addTab(rbox,tr("Preview"));
    tab->addTab(expertool,tr("Experiment"));
    lay->addWidget(tab);
#else
    lay->addWidget(rbox);
#endif
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

    photocanvas->setStrategy(PhotoCanvas::SinglePixmap);
#ifdef uselabelcanvas
    livecanvas->setAlignment(Qt::AlignCenter);
    livecanvas->setScaledContents(true);
#else
    livecanvas->setStrategy(PhotoCanvas::SinglePixmap);
    livecanvas->optimizePaint();
#endif

#ifdef usetab
    tab->setMaximumWidth(PreviewToolBarMaxWidth);
#endif
}

void Preview::initObjects()
{
    cameramode = new CameraMode;
#ifdef uselabelcanvas
    livecanvas = new LabelTriangle;
#else
    livecanvas = new PhotoCanvasTriangle;
#endif
#ifdef usetab
    tab = new QTabWidget;
    expertool = new ExperTool;
#endif
    photocanvas = new PhotoCanvas;
    stack = new QStackedWidget;
    pattern = new WellPattern(2,3);
    groupinfo = new GroupInfo;
    previewtool = new PreviewTool;
    viewpattern = new ViewPattern;// 视野窗口
    dock = new DockWidget(tr("Select Hole Inside View"));
    dockcanvas = new QMainWindow;
    scrollarea = new QScrollArea;
    Toupcam_GigeEnable(nullptr, nullptr);// 初始化对相机的支持
}

void Preview::initConnections()
{
    connect(previewtool,&PreviewTool::manufacturerChanged,this,&Preview::onManufacturerChanged);
    connect(previewtool,&PreviewTool::wellbrandChanged,this,&Preview::onWellbrandChanged);
    connect(previewtool,&PreviewTool::objectiveChanged,this,&Preview::onObjectiveChanged);
    connect(previewtool,&PreviewTool::cameraAdjusted,this,&Preview::adjustCamera);
    connect(previewtool,&PreviewTool::photoTaking,this,&Preview::takingPhoto);
    connect(previewtool,&PreviewTool::directionMove,this,&Preview::adjustLens);
    connect(previewtool,&PreviewTool::channelChanged,this,&Preview::toggleChannel);
    connect(previewtool,&PreviewTool::exportFilePath,this,&Preview::exportExperConfig);
    connect(previewtool,&PreviewTool::importFilePath,this,&Preview::importExperConfig);
    connect(previewtool,&PreviewTool::loadExper,this,&Preview::loadExper);

    connect(ParserPointer,&ParserControl::parseResult,this,&Preview::onAdjustCamera);
#ifdef notusetoupcamera
    connect(this, &Preview::evtCallback, this, &Preview::processCallback);
#else
    connect(ToupCameraPointer,&ToupCamera::imageCaptured,this,&Preview::showCapturedImage);
    connect(ToupCameraPointer,&ToupCamera::exposureGainCaptured,previewtool,&PreviewTool::exposureGainCaptured);
#endif

    connect(cameramode,&CameraMode::cameraModeChanged,this,[=](int option){stack->setCurrentIndex(option);});
#ifdef uselabelcanvas
    connect(livecanvas,&LabelTriangle::triangleClicked,this,&Preview::adjustLens);
#else
    connect(livecanvas,&PhotoCanvasTriangle::triangleClicked,this,&Preview::adjustLens);
#endif

    connect(pattern,&WellPattern::openViewWindow,this,&Preview::updateViewWindow); // 打开和更新视野窗口
    connect(pattern,&WellPattern::openSetGroupWindow,this,&Preview::updateSetGroupWindow);// 打开分组窗口
    connect(pattern,&WellPattern::mouseClicked,this,&Preview::previewViewByClickHole); // 点击孔也触发预览
    connect(pattern,&WellPattern::clearViewWindowCache,viewpattern,&ViewPattern::clearViewWindowCache);//删孔时清除该孔的缓存信息

    connect(viewpattern,&ViewPattern::applyHoleEvent,pattern,&WellPattern::updateHoleInfoByViewInfoApplyHole);//删点或者保存点就应用到本孔
    connect(viewpattern,&ViewPattern::applyGroupEvent,pattern,&WellPattern::updateHoleInfoByViewInfoApplyGroup); // 按组更新孔窗口的信息
    connect(viewpattern,&ViewPattern::applyAllEvent,pattern,&WellPattern::updateHoleInfoByViewInfoApplyAll); // 不按组更新孔窗口的信息
    connect(viewpattern,&ViewPattern::previewEvent,this,&Preview::previewViewByClickView); // 点击视野预览

    connect(this,&Preview::objectiveSettingChanged,previewtool,&PreviewTool::objectiveSettingChanged);
#ifdef usetab
    connect(previewtool,&PreviewTool::objectiveChanged,expertool,&ExperTool::objectiveChanged);
    connect(expertool,&ExperTool::exportFilePath,this,&Preview::exportExperConfig);
    connect(expertool,&ExperTool::loadExper,this,&Preview::loadExper);
#endif
#ifndef notusetoupcamera
//    connect(&timer,&QTimer::timeout,this,[=]{
//        QImage img1(CURRENT_PATH+"/images/cell.png");
//        QImage img2(CURRENT_PATH+"/images/test.jpeg");
//        static long count = 0;
//        count %2 ? showCapturedImage(img1):showCapturedImage(img2);
//        count ++;
//    });
//    timer.start(10);
#endif
}