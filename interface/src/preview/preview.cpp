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

void Preview::setViewMode(int mode)
{
    switch (mode) {
        case 1:
            wellview->setViewMode(ViewMode::PointMode);
            wellpattern->setViewMode(ViewMode::PointMode);
            break;
        case 2:
            wellview->setViewMode(ViewMode::RectMode);
            wellpattern->setViewMode(ViewMode::RectMode);
            break;
        case 3:
            wellview->setViewMode(ViewMode::WholeMode);
            wellpattern->setViewMode(ViewMode::WholeMode);
            break;
    }
}

void Preview::toggleStack(int option) 
{ // 切换孔板和载玻片,0-plate,1-slide
    stack_view_pattern->setCurrentWidget(stackpattern); // 无论是载玻片还是孔板都是显示pattern而不是view
    stackpattern->setCurrentIndex(option);
    //wellview->setEnabled(!option); // 不去控制,只控制slidepattern,这2个只能由2个按钮去控制
    //wellpattern->setEnabled(option);
    slidepattern->setEnabled(option);

    if (option) { // 载玻片
        canvasmode->changeMode(CanvasMode::PhotoMode);
        photocanvas->setStrategy(PhotoCanvas::GridPixmap);
    } else {
        canvasmode->changeMode(CanvasMode::LiveMode);
        photocanvas->setStrategy(PhotoCanvas::SinglePixmap);
    }
}

void Preview::toggleBrand(int option)
{ // 切换厂家的话,需要切换堆叠和相应的显示,堆叠界面的切换由另一个信号负责=>welltypeChanged=>toggleStack
    stack_view_pattern->setCurrentWidget(stackpattern);
    switch (option) {
        case 0:
            wellpattern->setPatternSize(2,3);
            stackpattern->setCurrentWidget(wellpattern);
            break;
        case 1:
            wellpattern->setPatternSize(4,6);
            stackpattern->setCurrentWidget(wellpattern);
            break;
        case 2:
            wellpattern->setPatternSize(8,12);
            stackpattern->setCurrentWidget(wellpattern);
            break;
        case 3:
            wellpattern->setPatternSize(16,24);
            stackpattern->setCurrentWidget(wellpattern);
            break;
        case SlideIndexInBrand:
            stackpattern->setCurrentWidget(slidepattern);
            break;
    }

    // 1.更新视野的尺寸
    auto objective = previewtool->currentObjective();
    auto brand = wellbox->wellBrand();
    auto manufacturer = wellbox->wellManufacturer();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];

    if (stackpattern->currentWidget() == wellpattern) {
        // 2. 更新视野窗口去更新视野窗口绘制和临时保存信息
#ifdef viewRowColUnEqual
        wellview->toggleBrandObjective(Dimension2D(size,size), false);
#else
        wellview->toggleBrandObjective(size, false);
#endif
        // 3. 视野窗口的数据信息临时信息需要更改,因为尺寸变了
        wellview->drawGroupText(tr(PreviewDockHoleTitle));

        // 4.切换厂家setPattenSize已经都清理完了,无需再调用

        // 5. 对NA物镜的特殊处理放最后,因为viewpattern->clearAllViewWindowCache被调用在前
        auto objective_descrip = previewtool->currentObjectiveDescription();
        if (objective_descrip == NA40x095Field) { // 只能选择1个孔
            wellpattern->setDisableHoles();
            wellpattern->setDisableHole(QPoint(0,0),false);//默认只允许(0,0)可选
        } else {
            wellpattern->setDisableHoles(false);
        }
        if (objective_descrip == NA20x05Field)
            wellview->setDisableRect(0.1);
        else if (objective_descrip == NA20x08Field)
            wellview->setDisableRect(0.2);
        else wellview->clearDisableRect();
    }
    else if (stackpattern->currentWidget() == slidepattern) {
        slidepattern->updateSize(size);
        photocanvas->setStrategy(PhotoCanvas::GridPixmap);
        photocanvas->setGridSize(size);
        photocanvas->clearGridImage();
        slidepattern->refreshRect();

    }
}

// 切换物镜的尺寸,视野尺寸要发生变化,不论是载玻片还是孔板都同时更新
void Preview::toggleObjective(const QString& obj)
{
    LOG<<"objective option = "<<obj;
    
    // (1) 更新wellpattern-wellview
    // 1.更新视野的尺寸
    auto objective = previewtool->currentObjective();
    auto brand = wellbox->wellBrand();
    auto manufacturer = wellbox->wellManufacturer();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    // 2. 更新视野窗口去更新视野窗口绘制和临时保存信息
#ifdef viewRowColUnEqual
    auto oldViewSize = wellview->viewInfo()[HoleViewSizeField].value<Dimension2D>();
    if (oldViewSize != Dimension2D(size,size)) { // 如果视野尺寸前后没发生变化,不需要清理
        LOG<<"toggle objective viewsize,before: "<<oldViewSize<<" after: "<<size<<" need clear";
        wellview->toggleBrandObjective(Dimension2D(size,size),true); // 清理ViewPattern
        wellview->drawGroupText(tr(PreviewDockHoleTitle));
        wellpattern->clearViewInfo();// 只需要孔关于视野的信息,其它保留
    } else {
        LOG<<"toggle objective viewsize,before: "<<oldViewSize<<" after: "<<size<<" not clear";
    }
#else
    auto oldViewSize = wellview->viewInfo()[HoleViewSizeField].toInt();
    if (oldViewSize != size) { // 如果视野尺寸前后没发生变化,不需要清理
        LOG<<"toggle objective viewsize,before: "<<oldViewSize<<" after: "<<size<<" need clear";
        wellview->toggleBrandObjective(size,true); // 清理ViewPattern
        wellview->drawGroupText(tr(PreviewDockHoleTitle));
        wellpattern->clearViewInfo();// 只需要孔关于视野的信息,其它保留
    } else {
        LOG<<"toggle objective viewsize,before: "<<oldViewSize<<" after: "<<size<<" not clear";
    }
#endif


    // 3. 对NA物镜的特殊处理要放在最后,因为上边的代码viewpattern->clearAllViewWindowCache会重新初始化视野尺寸
    if (obj == NA40x095Field) { // 只能选择1个孔
        wellpattern->setDisableHoles();
        wellpattern->setDisableHole(QPoint(0,0),false);//默认只允许(0,0)可选
    } else  wellpattern->setDisableHoles(false);

    if (obj == NA20x05Field)
        wellview->setDisableRect(0.1);
    else if (obj == NA20x08Field)
        wellview->setDisableRect(0.2);
    else wellview->clearDisableRect();
    
    // (2) 更新slidepattern
    slidepattern->updateSize(size);

    // (3) 对于slide而言photocanvas的拼图模式不同,在网格策略下需要更新尺寸
    if (photocanvas->strategy() == PhotoCanvas::GridPixmap)
    {
        photocanvas->setGridSize(size);
        photocanvas->clearGridImage();
        slidepattern->refreshRect();// 切物镜后把slidepattern的矩形重新刷新到photocanvas上
    }
}

// 打开wellview
void Preview::openWellViewWindow(const QVariantMap& m)
{
    stack_view_pattern->setCurrentWidget(wellview);
    // 1. 更新视野窗口的标题
    auto holepoint = m[HoleCoordinateField].toPoint();
    auto groupname = m[HoleGroupNameField].toString();
    if (groupname.isEmpty()) {
        groupname = tr(PreviewNoGroupWellDockTitle);
        auto gname = previewtool->currentGroup(); // camerabox的组别下拉框当前选项
        groupinfo->setGroupName(gname); // 如果点击的空是空的是可以分组的,要让此时的groupinfo要跟随当前的组别设置
    }

    // 2.根据当前brand/objective更新视野的尺寸
    wellview->drawGroupText(tr(PreviewWellDockTitle).arg(QChar(holepoint.x()+65)).arg(holepoint.y()+1).arg(groupname));

    auto objective = previewtool->currentObjective();
    auto brand = wellbox->wellBrand();
    auto manufacturer = wellbox->wellManufacturer();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];

    // 3. ⭐⭐⭐⭐ 把图案的信息传给视野窗口,必须这里额外组装3个字段
    auto nm = m;
#ifdef viewRowColUnEqual
    nm[HoleViewSizeField].setValue(Dimension2D(size,size));
#else
    nm[HoleViewSizeField] = size;
#endif

    nm[HoleViewRectsField].setValue(QRectFVector());
    nm[HoleViewUiPointsField].setValue(QPointFVector());
    nm[HoleViewPointsField].setValue(QPointFVector());

    if (holepoint != QPoint(-1,-1))
        wellview->setViewInfo(nm);

    // 6. 对NA物镜的特殊处理要放在最后,setViewInfo会重新初始化视野尺寸
    auto objective_descrip = previewtool->currentObjectiveDescription();
    if (objective_descrip == NA20x05Field)
        wellview->setDisableRect(0.1);
    else if (objective_descrip == NA20x08Field)
        wellview->setDisableRect(0.2);
    else wellview->clearDisableRect();
}

// 使用该孔对应的信息去更新分组窗口的UI
void Preview::openWellGroupWindow(const QVariantMap &m)
{
    // 1. 更新分组窗口的ui信息
    groupinfo->setGroupInfo(m);

    // 2. 分组窗口设置的组信息去更新孔的数据
    int ret = groupinfo->exec();
    if (ret == QDialog::Accepted ) {
        wellpattern->updateHoleInfo(groupinfo->groupInfo()); // 用当前的组信息去更新孔的颜色
    }
}

void Preview::showCapturedImage(const QImage& image)
{ // 来自照相机的捕捉图像事件传来的live图像显示
#ifdef uselabelcanvas
    auto pix = QPixmap::fromImage(img).scaled(livecanvas->size(),Qt::KeepAspectRatio,Qt::FastTransformation);
    livecanvas->setPixmap(pix);
    livecanvas->repaint();
#else
    livecanvas->setImage(image,10); // 10张图片显示1次
#endif
}

void Preview::setAppInfo(int app)
{ // 用于appselect传递当前app信息
    previewinfo[AppSelectField] = QString::number(app);
}

WellPatternInfo Preview::patternInfo() const
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
    canvasmode->changeMode(CanvasMode::VideoMode);
    if (path.isEmpty()) return;

    videocanvas->setMedia(path);
    videocanvas->play();
}

void Preview::stopVideo()
{
    canvasmode->changeMode(CanvasMode::VideoMode);
    videocanvas->stop();
}

void Preview::pauseVideo()
{
    canvasmode->changeMode(CanvasMode::VideoMode);
    videocanvas->pause();
}

void Preview::initLayout()
{
    // 1.右侧布局
    auto play = new QVBoxLayout;
    play->addWidget(stack_view_pattern);
    holebox->setLayout(play);
    auto rlay = new QVBoxLayout;
    rlay->addWidget(historybox);// 1.1
    rlay->addWidget(wellbox);// 1.2
    rlay->addWidget(holebox);// 1.3
    rlay->addWidget(scrollarea); // 1.4
    scrollarea->setWidget(previewtool);

    auto rbox = new GroupBox;
    rbox->setLayout(rlay);
    rbox->setMaximumWidth(PreviewToolBarMaxWidth);

    // 2.左侧布局
    auto llay = new QVBoxLayout;
    llay->addWidget(canvasmode);
    llay->addWidget(stackcanvas);
    auto lbox = new GroupBox;
    lbox->setLayout(llay);

    // 3.总布局
    tab->addTab(rbox,tr("Preview"));
    tab->addTab(expertool,tr("Experiment"));

    auto lay = new QHBoxLayout;
//    lay->addWidget(lbox);
//    lay->addWidget(tab);
    auto splitter = new Splitter(Qt::Horizontal);
    splitter->addWidget(lbox);
    splitter->addWidget(tab);
    lay->addWidget(splitter);
    setLayout(lay);
}

void Preview::initAttributes()
{
    // 1. photo默认单图模式不启用线程
    photocanvas->setStrategy(PhotoCanvas::SinglePixmap);// 默认单图模式
    photocanvas->enableTransformThread(false);
    // 2. live默认单图模式启用线程并开启优化
#ifdef uselabelcanvas
    livecanvas->setAlignment(Qt::AlignCenter);
    livecanvas->setScaledContents(true);
#else
    livecanvas->drawTriangle(false);
    livecanvas->setStrategy(PhotoCanvas::SinglePixmap);
#ifdef use_imagetransformthread
    livecanvas->enableTransformThread(true);
#endif
    livecanvas->optimizePaint(50);
#endif
    // 3. 图像堆叠的界面
    stackcanvas->addWidget(livecanvas);
    stackcanvas->addWidget(photocanvas);
    stackcanvas->addWidget(videocanvas);

    // 4. 孔板/载玻片堆叠的界面
    wellpattern->setEnabled(false);
    stackpattern->addWidget(wellpattern);
    stackpattern->addWidget(slidepattern);
    stackpattern->setMinimumHeight(PreviewPatternMinHeight);

    wellview->drawTriangle(false);
    wellview->setEnabled(false);

    stack_view_pattern->addWidget(stackpattern);
    stack_view_pattern->addWidget(wellview); // stack=>stack1+wellview;stack1=>wellpattern+slidepattern

    tab->setMaximumWidth(PreviewToolBarMaxWidth);
    scrollarea->setWidgetResizable(true);
}

void Preview::initObjects()
{
    canvasmode = new CanvasMode;
    groupinfo = new GroupInfo;

#ifdef uselabelcanvas
    livecanvas = new LabelTriangle;
#else
    livecanvas = new PhotoCanvasTriangle;
#endif
    photocanvas = new PhotoCanvas;
    videocanvas = new VideoWidget;
    stackcanvas = new QStackedWidget;

    holebox = new GroupBox(tr("Hole/View Selection"));
    historybox = new HistoryBox;
    wellbox = new WellBox;

    wellpattern = new WellPattern(2,3);
    slidepattern = new SlidePattern;
    stackpattern = new QStackedWidget;

    wellview = new WellView;
    stack_view_pattern = new QStackedWidget;

    tab = new QTabWidget;
    expertool = new ExperTool;
    previewtool = new PreviewTool;
    scrollarea = new QScrollArea;
}

void Preview::initConnections()
{
    // (1) previewtool/expertool => preview 信号-槽函数直连
    connect(previewtool,&PreviewTool::objectiveChanged,this,&Preview::toggleObjective);
    connect(previewtool,&PreviewTool::objectiveToggled,this,&Preview::adjustObjective);
    connect(previewtool,&PreviewTool::playVideo,this,&Preview::playVideo);
    connect(previewtool,&PreviewTool::stopVideo,this,&Preview::stopVideo);
    connect(previewtool,&PreviewTool::pauseVideo,this,&Preview::pauseVideo);
    connect(previewtool,&PreviewTool::cameraAdjusted,this,&Preview::adjustCamera);
    connect(previewtool,&PreviewTool::brightAdjusted,this,&Preview::adjustBright);
    connect(previewtool,&PreviewTool::photoTaking,this,&Preview::takingPhoto);
    connect(previewtool,&PreviewTool::slideStitching,this,&Preview::stitchSlide);
    connect(previewtool,&PreviewTool::focusChanged,this,&Preview::adjustFocus);
    connect(previewtool,&PreviewTool::autoFocus,this,&Preview::autoFocus);
    connect(previewtool,&PreviewTool::channelChanged,this,&Preview::toggleChannel);
    connect(previewtool,&PreviewTool::channelClosed,this,&Preview::closeChannel);
    connect(previewtool,&PreviewTool::modeSelected,this,&Preview::setViewMode);
    connect(previewtool,&PreviewTool::objectiveChanged,expertool,&ExperTool::objectiveChanged);
    connect(previewtool,&PreviewTool::triangleClicked,this,&Preview::adjustLens);
    connect(previewtool,&PreviewTool::enableWellPattern,wellpattern,&WellPattern::setEnabled);
    connect(previewtool,&PreviewTool::enableWellPattern,wellview,&WellView::setEnabled);
    //connect(previewtool,&PreviewTool::enableWellPattern,holebox,&GroupBox::setVisible);//暂时不加
    connect(previewtool,&PreviewTool::groupTypeChanged,groupinfo,&GroupInfo::setGroupName);
    connect(previewtool,&PreviewTool::groupColorChanged,groupinfo,&GroupInfo::setGroupColor);
    connect(expertool,&ExperTool::exportFilePath,this,&Preview::exportExperConfig);
    connect(expertool,&ExperTool::loadExper,this,&Preview::loadExper);
    connect(expertool,&ExperTool::stopExper,this,&Preview::stopExper);

    // (2) preview内部信号-槽函数
    connect(canvasmode,&CanvasMode::cameraModeChanged,this,[=](int option){stackcanvas->setCurrentIndex(option);});
#ifdef uselabelcanvas
    connect(livecanvas,&LabelTriangle::triangleClicked,this,&Preview::adjustLens);
#endif
    connect(historybox,&HistoryBox::importFilePath,this,&Preview::importExperConfig);
    connect(wellbox,&WellBox::wellbrandChanged,this,&Preview::toggleBrand);
    connect(wellbox,&WellBox::welltypeChanged,this,&Preview::toggleStack);
    connect(wellbox,&WellBox::welltypeChanged,previewtool,&PreviewTool::wellbrandChanged);

    connect(wellpattern,&WellPattern::openWellViewWindow,this,&Preview::openWellViewWindow); // 打开和更新视野窗口
    connect(wellpattern,&WellPattern::openWellGroupWindow,this,&Preview::openWellGroupWindow);// 打开分组窗口
    connect(wellpattern,&WellPattern::holeClicked,this,&Preview::previewHoleEvent); // 点击孔也触发预览
    connect(wellpattern,&WellPattern::removeHole,wellview,&WellView::removeHole);//删孔时清除该孔的缓存信息
    connect(wellpattern,&WellPattern::toggleWellStack,wellview,&WellView::quitView);
    connect(wellpattern,&WellPattern::groupChanged,previewtool,&PreviewTool::groupChanged);
    connect(slidepattern,&SlidePattern::previewEvent,this,&Preview::previewSlideEvent);
    connect(slidepattern,&SlidePattern::normRectUpdated,photocanvas,&PhotoCanvas::updateRect);

    connect(wellview,&WellView::applyHoleEvent,wellpattern,&WellPattern::applyHoleEvent);//删点或者保存点就应用到本孔
    connect(wellview,&WellView::applyGroupEvent,wellpattern,&WellPattern::applyGroupEvent); // 按组更新孔窗口的信息
    connect(wellview,&WellView::applyAllEvent,wellpattern,&WellPattern::applyAllEvent); // 不按组更新孔窗口的信息
    connect(wellview,&WellView::selectModeChangedEvent,wellpattern,&WellPattern::selectModeChangedEvent);
    connect(wellview,&WellView::previewEvent,this,&Preview::previewViewEvent); // 点击视野预览
    //connect(wellview,&WellView::triangleClicked,this,&Preview::adjustLens); // 点模式微调,取消在这里,改为viewmode那里去调
    connect(wellview,&WellView::quitView,[=]{stack_view_pattern->setCurrentWidget(stackpattern);});

    connect(groupinfo,&GroupInfo::groupSetted,previewtool,&PreviewTool::groupSetted);

    // (3) 外部信号=>preview/previewtool
    connect(ParserPointer,&ParserControl::parseResult,this,&Preview::parseResult);
    connect(ToupCameraPointer,&ToupCamera::imageCaptured,this,&Preview::showCapturedImage);
    connect(ToupCameraPointer,&ToupCamera::imageCaptured,previewtool,&PreviewTool::imageCaptured);
    connect(ToupCameraPointer,&ToupCamera::exposureGainCaptured,previewtool,&PreviewTool::exposureGainCaptured);
    connect(this,&Preview::objectiveSettingChanged,previewtool,&PreviewTool::objectiveSettingChanged);//调整物镜位置
}