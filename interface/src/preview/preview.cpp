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

void Preview::setSelectMode(int mode)
{
    switch (mode) {
        case 0:
            wellview->setSelectMode(WellView::PointMode);
            wellpattern->setSelectMode(WellPattern::PointMode);
            break;
        case 1:
            wellview->setSelectMode(WellView::RectMode);
            wellpattern->setSelectMode(WellPattern::RectMode);
            break;
        case 2:
            wellview->setSelectMode(WellView::WholeMode);
            wellpattern->setSelectMode(WellPattern::WholeMode);
            break;
    }
}

void Preview::toggleManufacturer(int option)
{

}

void Preview::toggleStack(int option) 
{
    stackpattern->setCurrentIndex(option);
    stackview->setCurrentIndex(option);
    if (option) dock->setFloating(false);
}

void Preview::toggleBrand(int option)
{ // 切换厂家的话,需要切换堆叠和相应的显示,堆叠界面的切换由另一个信号负责=>welltypeChanged=>toggleStack
    switch (option) {
        case 0:
            wellpattern->setPatternSize(2,3);
            break;
        case 1:
            wellpattern->setPatternSize(4,6);
            break;
        case 2:
            wellpattern->setPatternSize(8,12);
            break;
        case 3:
            wellpattern->setPatternSize(16,24);
            break;
        case 4:
            break;
    }

    if (option == 4) { // 切换到slide会发现这里其实还是well
        stackpattern->setCurrentWidget(slidepattern);
        stackview->setCurrentWidget(slideview);
    } else {
        stackpattern->setCurrentWidget(wellpattern);
        stackview->setCurrentWidget(wellview);
    }

    // 1.更新视野的尺寸
    auto toolinfo = previewtool->toolInfo();
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];

    if (stackpattern->currentWidget() == wellpattern) {
        Q_ASSERT(stackview->currentWidget() == wellview);
        if (size > view_well_6_4x*10)
            dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
        else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
        else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);

        // 2. 更新视野窗口去更新视野窗口绘制和临时保存信息
        //LOG<<manufacturer<<brand<<objective<<size;
        wellview->toggleBrandObjective(size, false);

        // 3. 视野窗口的数据信息临时信息需要更改,因为尺寸变了
        dock->setWindowTitle(tr(PreviewDockHoleTitle));

        // 4.切换厂家setPattenSize已经都清理完了,无需再调用

        // 5. 对NA物镜的特殊处理放最后,因为viewpattern->clearAllViewWindowCache被调用在前
        auto objective_descrip = toolinfo[ObjectiveDescripField].toString();
        if (objective_descrip == NA40x095Field) { // 只能选择1个孔
            wellpattern->setDisablePoints();
            wellpattern->setDisablePoint(QPoint(0,0),false);//默认只允许(0,0)可选
        } else {
            wellpattern->setDisablePoints(false);
        }
        if (objective_descrip == NA20x05Field)
            wellview->setDisablePoints(NA20x05DisablePoints);
        else if (objective_descrip == NA20x08Field)
            wellview->setDisablePoints(NA20x08DisablePoints);
        else wellview->setDisablePoints(false);
    } else if (stackpattern->currentWidget() == slidepattern) {

        Q_ASSERT(stackview->currentWidget() == slideview);
        slideview->updateSize(size);
        slidepattern->updateSize(size);
    }
}

// 切换物镜的尺寸,视野尺寸要发生变化,不论是载玻片还是孔板都同时更新
void Preview::onObjectiveChanged(const QString& obj)
{
    LOG<<"objective option = "<<obj;
    
    // (1) 更新wellpattern-wellview
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
    auto oldViewSize = wellview->viewInfo()[HoleViewSizeField].toInt();

    if (oldViewSize != size) { // 如果视野尺寸前后没发生变化,不需要清理
        LOG<<"切换物镜前视野尺寸: "<<oldViewSize<<" 切换物镜后尺寸: "<<size<<" 需要清理";
        wellview->toggleBrandObjective(size,true); // 清理ViewPattern
        dock->setWindowTitle(tr("Select Hole Inside View"));
        wellpattern->clearAllHoleViewPoints();// 只需要孔关于视野的信息,其它保留
    } else {
        LOG<<"切换物镜前视野尺寸: "<<oldViewSize<<" 切换物镜后尺寸: "<<size<<" 无需清理";
    }

    // 3. 对NA物镜的特殊处理要放在最后,因为上边的代码viewpattern->clearAllViewWindowCache会重新初始化视野尺寸
    if (obj == NA40x095Field) { // 只能选择1个孔
        wellpattern->setDisablePoints();
        wellpattern->setDisablePoint(QPoint(0,0),false);//默认只允许(0,0)可选
    } else  wellpattern->setDisablePoints(false);

    if (obj == NA20x05Field)
        wellview->setDisablePoints(NA20x05DisablePoints);
    else if (obj == NA20x08Field)
        wellview->setDisablePoints(NA20x08DisablePoints);
    else wellview->setDisablePoints(false);
    
    // (2) 更新slidepattern
    slideview->updateSize(size);
    slidepattern->updateSize(size);
}

// 打开wellview
void Preview::openWellViewWindow(const QVariantMap& m)
{
    // 1. 更新视野窗口的标题
    auto holepoint = m[HoleCoordinateField].toPoint();
    auto groupname = m[HoleGroupNameField].toString();
    if (groupname.isEmpty())
        groupname = tr(PreviewNoGroupWellDockTitle);

    dock->setWindowTitle(tr(PreviewWellDockTitle)
                                 .arg(QChar(holepoint.x()+65)).arg(holepoint.y()+1).arg(groupname));
    dock->setFloating(true);

    // 2.根据当前brand/objective更新视野的尺寸
    auto toolinfo = previewtool->toolInfo();
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);
    //LOG<<" manufacturer = "<<manufacturer<<" brand = "<<brand<<" objective = "<<objective<<" size = "<<size;

    // 3. ⭐⭐⭐⭐ 把图案的信息传给视野窗口,必须这里额外组装3个字段
    auto nm = m;
    nm[HoleViewSizeField] = size;
    nm[HoleViewRectsField].setValue(QRectFVector());
    nm[HoleViewUiPointsField].setValue(QPointFVector());
    nm[HoleViewPointsField].setValue(QPointFVector());

    if (holepoint != QPoint(-1,-1))
        wellview->setViewInfo(nm);
//
//    // 6. 对NA物镜的特殊处理要放在最后,setStrategy会重新初始化视野尺寸
//    auto objective_descrip = toolinfo[ObjectiveDescripField].toString();
//    if (objective_descrip == NA20x05Field)
//        wellview->setDisablePoints(NA20x05DisablePoints);
//    else if (objective_descrip == NA20x08Field)
//        wellview->setDisablePoints(NA20x08DisablePoints);
//    else wellview->setDisablePoints(false);
}

// 打开slideview
void Preview::openSlideViewWindow()
{
    // 1.打开dock,此时堆叠界面是slideview
    Q_ASSERT(stackview->currentWidget() == slideview);
    dock->setWindowTitle(tr(PreviewDockSlideTitle));
    dock->setFloating(true);
    dock->setWindowSize(DefaultWindowSize.width()*2,DefaultWindowSize.height()*2);
    // 2. slidepattern不对slideview传递信息,也没有其它额外设置
}

// 使用该孔对应的信息去更新分组窗口的UI
void Preview::openWellGroupWindow(const QVariantMap &m)
{
    // 1. 更新分组窗口的ui信息
    groupinfo->setGroupInfo(m);

    // 2. 分组窗口设置的组信息去更新孔的数据
    int ret = groupinfo->exec();
    if (ret == QDialog::Accepted) {
        wellpattern->updateHoleInfoByGroupInfo(groupinfo->groupInfo()); // 用当前的组信息去更新孔的颜色
    }
}

void Preview::setAppInfo(int app)
{ // 用于appselect传递当前app信息
    previewinfo[AppSelectField] = QString::number(app);
}

PreviewPatternInfo Preview::patternInfo() const
{
    return wellpattern->patternInfo();
}

PreviewToolInfo Preview::toolInfo() const
{
    return previewtool->toolInfo();
}

void Preview::playVideo(const QString& path)
{
    LOG<<"video path = "<<path;
    cameramode->changeMode(CameraMode::VideoMode);
    if (path.isEmpty()) return;

    videocanvas->setMedia(path);
    videocanvas->play();
}

void Preview::stopVideo()
{
    cameramode->changeMode(CameraMode::VideoMode);
    videocanvas->stop();
}

void Preview::pauseVideo()
{
    cameramode->changeMode(CameraMode::VideoMode);
    videocanvas->pause();
}

void Preview::initLayout()
{
    // 1.右侧布局
    auto pbox = new GroupBox(tr("Hole Selection"));
    auto play = new QVBoxLayout;
    play->addWidget(stackpattern);
    pbox->setLayout(play);

    auto rlay = new QVBoxLayout;
    rlay->addWidget(pbox);
    rlay->addWidget(scrollarea); // 添加滚动区域后(不是添加previewtool)再设置
    scrollarea->setWidget(previewtool);

    auto rbox = new GroupBox;
    rbox->setLayout(rlay);
    rbox->setMaximumWidth(PreviewToolBarMaxWidth);

    // 2.左侧布局
    auto llay = new QVBoxLayout;
    llay->addWidget(cameramode);
    llay->addWidget(stackcanvas);
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
    //photocanvas->setStrategy(PhotoCanvas::SinglePixmap);// 默认单图模式
    photocanvas->setStrategy(PhotoCanvas::GridPixmap);
#ifdef uselabelcanvas
    livecanvas->setAlignment(Qt::AlignCenter);
    livecanvas->setScaledContents(true);
#else
    livecanvas->setStrategy(PhotoCanvas::SinglePixmap);
    livecanvas->optimizePaint(50);
#endif
    stackcanvas->addWidget(livecanvas);
    stackcanvas->addWidget(photocanvas);
    stackcanvas->addWidget(videocanvas);

    stackpattern->addWidget(wellpattern);
    stackpattern->addWidget(slidepattern);
    stackpattern->setMinimumHeight(PreviewPatternMinHeight);

    stackview->addWidget(wellview);
    stackview->addWidget(slideview);

    dock->setWidget(stackview);
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockcanvas->setCentralWidget(dock);
    dockcanvas->addDockWidget(Qt::BottomDockWidgetArea,dock);// 这个地方不能选all/no,否则无法实现点击隐藏关闭效果

#ifdef usetab
    tab->setMaximumWidth(PreviewToolBarMaxWidth);
#endif
    scrollarea->setWidgetResizable(true);
}

void Preview::initObjects()
{
    cameramode = new CameraMode;
    groupinfo = new GroupInfo;

#ifdef uselabelcanvas
    livecanvas = new LabelTriangle;
#else
    livecanvas = new PhotoCanvasTriangle;
#endif
    photocanvas = new PhotoCanvas;
    videocanvas = new VideoWidget;
    stackcanvas = new QStackedWidget;

    wellpattern = new WellPattern(2,3);
    slidepattern = new SlidePattern;
    stackpattern = new QStackedWidget;

    wellview = new WellView;
    slideview = new SlidePattern;
    stackview = new QStackedWidget;
    dock = new DockWidget(tr(PreviewDockHoleTitle));
    dockcanvas = new QMainWindow;

#ifdef usetab
    tab = new QTabWidget;
    expertool = new ExperTool;
#endif
    previewtool = new PreviewTool;
    scrollarea = new QScrollArea;

    Toupcam_GigeEnable(nullptr, nullptr);// 初始化对相机的支持
}

void Preview::initConnections()
{
    // (1) previewtool/expertool => preview 信号-槽函数直连
    connect(previewtool,&PreviewTool::manufacturerChanged,this,&Preview::toggleManufacturer);
    connect(previewtool,&PreviewTool::wellbrandChanged,this,&Preview::toggleBrand);
    connect(previewtool,&PreviewTool::welltypeChanged,this,&Preview::toggleStack);
    connect(previewtool,&PreviewTool::objectiveChanged,this,&Preview::onObjectiveChanged);
    connect(previewtool,&PreviewTool::objectiveToggled,this,&Preview::toggleObjective);
    connect(previewtool,&PreviewTool::playVideo,this,&Preview::playVideo);
    connect(previewtool,&PreviewTool::stopVideo,this,&Preview::stopVideo);
    connect(previewtool,&PreviewTool::pauseVideo,this,&Preview::pauseVideo);
    connect(previewtool,&PreviewTool::cameraAdjusted,this,&Preview::adjustCamera);
    connect(previewtool,&PreviewTool::photoTaking,this,&Preview::takingPhoto);
    connect(previewtool,&PreviewTool::directionMove,this,&Preview::adjustLens);
    connect(previewtool,&PreviewTool::focusChanged,this,&Preview::manualFocus);
    connect(previewtool,&PreviewTool::channelChanged,this,&Preview::toggleChannel);
    connect(previewtool,&PreviewTool::exportFilePath,this,&Preview::exportExperConfig);
    connect(previewtool,&PreviewTool::importFilePath,this,&Preview::importExperConfig);
    connect(previewtool,&PreviewTool::loadExper,this,&Preview::loadExper);
    connect(previewtool,&PreviewTool::stopExper,this,&Preview::stopExper);
    connect(previewtool,&PreviewTool::modeSelected,this,&Preview::setSelectMode);
#ifdef usetab
    connect(previewtool,&PreviewTool::objectiveChanged,expertool,&ExperTool::objectiveChanged);
    connect(expertool,&ExperTool::exportFilePath,this,&Preview::exportExperConfig);
    connect(expertool,&ExperTool::loadExper,this,&Preview::loadExper);
    connect(expertool,&ExperTool::stopExper,this,&Preview::stopExper);
#endif

    // (2) preview内部信号-槽函数
    connect(cameramode,&CameraMode::cameraModeChanged,this,[=](int option){stackcanvas->setCurrentIndex(option);});
#ifdef uselabelcanvas
    connect(livecanvas,&LabelTriangle::triangleClicked,this,&Preview::adjustLens);
#else
    connect(livecanvas,&PhotoCanvasTriangle::triangleClicked,this,&Preview::adjustLens);
#endif

    connect(wellpattern,&WellPattern::openWellViewWindow,this,&Preview::openWellViewWindow); // 打开和更新视野窗口
    connect(wellpattern,&WellPattern::openWellGroupWindow,this,&Preview::openWellGroupWindow);// 打开分组窗口
    connect(wellpattern,&WellPattern::mouseClicked,this,&Preview::previewHoleEvent); // 点击孔也触发预览
    connect(wellpattern,&WellPattern::removeHole,wellview,&WellView::removeHole);//删孔时清除该孔的缓存信息
    //connect(slidepattern,&SlidePattern::doubleClicked,this,&Preview::openSlideViewWindow);
    connect(slidepattern,&SlidePattern::rectUpdated,slideview,&SlidePattern::updateRect);

    connect(wellview,&WellView::applyHoleEvent,wellpattern,&WellPattern::applyHoleEvent);//删点或者保存点就应用到本孔
    connect(wellview,&WellView::applyGroupEvent,wellpattern,&WellPattern::applyGroupEvent); // 按组更新孔窗口的信息
    connect(wellview,&WellView::applyAllEvent,wellpattern,&WellPattern::applyAllEvent); // 不按组更新孔窗口的信息
    connect(wellview,&WellView::previewEvent,this,&Preview::previewViewEvent); // 点击视野预览
    connect(slideview,&SlidePattern::rectUpdated,slidepattern,&SlidePattern::updateRect);

    // (3) 外部信号=>preview/previewtool
    connect(ParserPointer,&ParserControl::parseResult,this,&Preview::onAdjustCamera);
#ifdef notusetoupcamera
    connect(this, &Preview::evtCallback, this, &Preview::processCallback);
#else
    connect(ToupCameraPointer,&ToupCamera::imageCaptured,this,&Preview::showCapturedImage);
    connect(ToupCameraPointer,&ToupCamera::imageCaptured,previewtool,&PreviewTool::imageCaptured);
    connect(ToupCameraPointer,&ToupCamera::exposureGainCaptured,previewtool,&PreviewTool::exposureGainCaptured);
#endif
    connect(this,&Preview::objectiveSettingChanged,previewtool,&PreviewTool::objectiveSettingChanged);//调整物镜位置


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