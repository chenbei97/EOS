/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\preview.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

Preview::Preview(QWidget*parent): QWidget(parent)
{
    cameramode = new CameraMode;
    livecanvas = new QWidget;
    photocanvas = new PreviewPhotoCanvas;
    stack = new QStackedWidget;
    pattern = new WellPattern(2,3);
    groupinfo = new GroupInfo;
    toolbar = new PreviewTool;
    viewpattern = new PreviewPhotoCanvas;//侧边栏选择视野的窗口
    dock = new DockWidget(tr("选择孔内视野"));
    dockcanvas = new QMainWindow;

    dock->setWidget(viewpattern);
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockcanvas->setCentralWidget(dock);
    dockcanvas->addDockWidget(Qt::BottomDockWidgetArea,dock);// 这个地方不能选all/no,否则无法实现点击隐藏关闭效果

    // 右侧布局
    pattern->setMinimumHeight(PreviewPatternMinHeight);
    auto pbox = new GroupBox(tr("选孔"));
    auto play = new QVBoxLayout;
    play->addWidget(pattern);
    //play->addWidget(dockcanvas); // 不加了通过点击孔来触发
    pbox->setLayout(play);

    auto rlay = new QVBoxLayout;
    rlay->addWidget(pbox);
    rlay->addWidget(toolbar);
    auto rbox = new GroupBox;
    rbox->setLayout(rlay);
    rbox->setMaximumWidth(PreviewToolBarMaxWidth);

    // 左侧布局
    stack->addWidget(livecanvas);
    stack->addWidget(photocanvas);
    auto llay = new QVBoxLayout;
    llay->addWidget(cameramode);
    llay->addWidget(stack);
    auto lbox = new GroupBox;
    lbox->setLayout(llay);

    // 总布局
    auto lay = new QHBoxLayout;
    lay->addWidget(lbox);
    lay->addWidget(rbox);
    setLayout(lay);

    connect(toolbar,&PreviewTool::manufacturerChanged,this,&Preview::onManufacturerChanged);
    connect(toolbar,&PreviewTool::wellbrandChanged,this,&Preview::onWellbrandChanged);
    connect(toolbar,&PreviewTool::objectiveChanged,this,&Preview::onObjectiveChanged);
    connect(toolbar,&PreviewTool::infoChanged,this,&Preview::onInfoChanged);
    connect(pattern,&WellPattern::viewEvent,this,&Preview::onViewEvent);
    connect(pattern,&WellPattern::doubleClicked,this,&Preview::onViewEvent);
    connect(pattern,&WellPattern::drapEvent,this,&Preview::onDrapEvent);
    connect(cameramode,&CameraMode::cameraModeChanged,this,[=](int option){stack->setCurrentIndex(option);});
    //connect(dock,&QDockWidget::topLevelChanged,this,&Preview::updatePattern);
}

void Preview::updateViewPattern()
{
    auto objective = getIndexFromFields(toolbar->toolInfo()[ObjectiveField].toString()).toUInt();
    auto brand = getIndexFromFields(toolbar->toolInfo()[BrandField].toString()).toUInt();
    auto manufacturer = getIndexFromFields(toolbar->toolInfo()[ManufacturerField].toString()).toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    viewpattern->setStrategy(PreviewPhotoCanvas::InnerCircleRect,size,size);

    LOG<<" manufacturer = "<<manufacturer<<" brand = "<<brand<<"objective = "<<objective<<" size = "<<size
       <<" current hole = "<<pattern->currentMousePoint(); // 有可能是没选孔(不过已经避免了brand,objective的触发了不会发生)

    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);
}

void Preview::onViewEvent(const QPoint &point)
{ // 点击图案的某个点,就要切换到photo模式,并依据当前的objective,brand设置photocanvas的绘制策略
    //cameramode->changeMode(CameraMode::PhotoMode);

    // 关闭相机,切换stack
    //stack->setCurrentWidget(photocanvas);
    dock->setWindowTitle(tr("选择孔内视野(%1,%2)")
        .arg(QChar(point.x()+65)).arg(point.y()+1));
    dock->setFloating(true);
    if (point == QPoint(-1,-1))
        viewpattern->setStrategy(PreviewPhotoCanvas::NoStrategy);
    else updateViewPattern();
}

void Preview::onDrapEvent(const QVariantMap& m)
{
    auto color = m[GroupColorField].toString();
    auto group = m[GroupNameField].toString();

    groupinfo->setGroupColor(color);// color是传过来之前的颜色
    groupinfo->setGroupName(group);


    int ret = groupinfo->exec();
    if (ret == QDialog::Accepted) {
        pattern->setGroup(groupinfo->groupInfo()); // 用选择的颜色去分组
    }
}

void Preview::onManufacturerChanged(int option)
{

}

void Preview::onWellbrandChanged(int option)
{
    updateViewPattern(); // 会造成没选孔就选了点会出问题
    //dock->setFloating(true);
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

void Preview::onObjectiveChanged(int option)
{
    updateViewPattern();
    //dock->setFloating(true);
}

void Preview::onInfoChanged()
{   // 任意改变都会更新信息,这个信息结合pattern的信息来组装数据,无需保存数据库,只需要下发指令
    auto toolinfo = toolbar->toolInfo();

    //AssemblerPointer->assemble(TcpFramePool.frame0x0001,toolinfo);

    //LOG<<"request = "<<AssemblerMessage;

    //SocketPointer->exec(TcpFramePool.frame0x0001,AssemblerMessage);

//    QFile file(CURRENT_PATH+"/../test/test0x0001.json");
//    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
//    QTextStream s(&file);
//    s<<AssemblerMessage;
//    file.close();
}