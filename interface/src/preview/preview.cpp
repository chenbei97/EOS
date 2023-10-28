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
    connect(cameramode,&CameraMode::cameraModeChanged,this,[=](int option){stack->setCurrentIndex(option);});

    connect(pattern,&WellPattern::openViewWindow,this,&Preview::updateViewWindow); // 打开和更新视野窗口
    connect(pattern,&WellPattern::openSetGroupWindow,this,&Preview::openSetGroupWindow);// 打开分组窗口
    connect(viewpattern,&PreviewPhotoCanvas::applyGroupEvent,pattern,&WellPattern::updateGroupByViewInfo); // 更新孔窗口的分组信息

    //connect(dock,&QDockWidget::topLevelChanged,this,&Preview::updatePattern);
}

void Preview::updateViewWindow(const QVariantMap& m)
{ // 打开选视野的窗口,更新视野的信息(groupname,groupcolor,grouppoints,coordinate+allgroup)+viewsize

    // 1. 相机模式的切换 live/photo => 切换stack,关闭相机等动作
    //cameramode->changeMode(CameraMode::PhotoMode);
    //stack->setCurrentWidget(photocanvas);

    // 2. 更新视野窗口的标题
    auto point = m[CoordinateField].toPoint(); //双击或者右键打开视野窗口带来的孔信息
    auto groupname = m[GroupNameField].toString();
    if (groupname.isEmpty()) groupname = tr("未设置组");
    dock->setWindowTitle(tr("选择孔内视野(%1,%2)-组别(%3)")
        .arg(QChar(point.x()+65)).arg(point.y()+1).arg(groupname));
    dock->setFloating(true);

    // 3.根据当前brand/objective更新视野的尺寸
    auto objective = getIndexFromFields(toolbar->toolInfo()[ObjectiveField].toString()).toUInt();
    auto brand = getIndexFromFields(toolbar->toolInfo()[BrandField].toString()).toUInt();
    auto manufacturer = getIndexFromFields(toolbar->toolInfo()[ManufacturerField].toString()).toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);
    //    LOG<<" manufacturer = "<<manufacturer<<" brand = "<<brand<<"objective = "<<objective<<" size = "<<size
//       <<" current hole = "<<pattern->currentMousePoint(); // 有可能是没选孔(不过已经避免了brand,objective的触发了不会发生)

    // 4. 更新视野绘制小正方形行列数
    auto nm = m;
    nm[ViewSizeField] = QSize(size,size);

    // 5. 鼠标点击边缘设置无策略或者有效时更新视野窗口策略
    if (point == QPoint(-1,-1))
        viewpattern->setStrategy(PreviewPhotoCanvas::NoStrategy);
    else viewpattern->setStrategy(PreviewPhotoCanvas::InnerCircleRect,nm);//⭐⭐⭐⭐ 把图案的信息传给视野窗口
}

void Preview::openSetGroupWindow(const QVariantMap& m)
{ // 打开分组窗口事件 会把当前孔所属组颜色/组名称(可能设置过)
    // 1. 更新分组窗口的ui信息
    auto color = m[GroupColorField].toString();
    auto group = m[GroupNameField].toString();
    groupinfo->setGroupColor(color);// color可能是设置过的组颜色,没设置过是红色
    groupinfo->setGroupName(group); // 组名称没设置过就是孔,更新分组窗口的UI信息

    // 2. 分组窗口设置的组信息去更新孔的数据
    int ret = groupinfo->exec();
    if (ret == QDialog::Accepted) {
        pattern->updateGroupByGroupInfo(groupinfo->groupInfo()); // 用当前的组信息去更新孔的颜色
    }
}

void Preview::onManufacturerChanged(int option)
{

}

void Preview::updateViewSize()
{
    // 1.更新视野的尺寸
    auto objective = getIndexFromFields(toolbar->toolInfo()[ObjectiveField].toString()).toUInt();
    auto brand = getIndexFromFields(toolbar->toolInfo()[BrandField].toString()).toUInt();
    auto manufacturer = getIndexFromFields(toolbar->toolInfo()[ManufacturerField].toString()).toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);

    // 2. 孔的信息不需要改变,仅仅需要更新视野窗口去更新视野窗口绘制和临时保存信息
    auto currentHoleInfo = viewpattern->currentHoleInfo();//拿到之前的设置,其它不用改
    currentHoleInfo[ViewSizeField] = QSize(size,size);
    viewpattern->setStrategy(PreviewPhotoCanvas::InnerCircleRect,currentHoleInfo); // 重新更新下尺寸信息,其他的不用改

    // 3. 视野窗口的数据信息临时信息需要更改,因为尺寸变了
    //viewpattern->updateApplyGroup(); // 重新刷新下分组信息更新孔图案,如果内部注释掉了pattern的更新代码,其余部分setStrategy-initSelectPoints其实操作过了

    // 4. 孔被选中的信息也需要更新,点的信息要更新
    //pattern->clearHolePattern();
    pattern->initHoleInfo(); // 区别是组名和组也要去除

}

void Preview::onWellbrandChanged(int option)
{
    updateViewSize(); // 会造成没选孔就选了点会出问题
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
    //LOG<<"objective option = "<<option;
    updateViewSize();
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