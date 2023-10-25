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
    pattern = new PreviewPattern(2,3);
    toolbar = new PreviewTool;

    // 右侧布局
    pattern->setMinimumHeight(PreviewPatternMinHeight);
    auto pbox = new GroupBox(tr("选孔"));
    auto play = new QVBoxLayout;
    play->addWidget(pattern);
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
    connect(pattern,&PreviewPattern::mouseClicked,this,&Preview::onClickPattern);
    connect(cameramode,&CameraMode::cameraModeChanged,this,[=](int option){stack->setCurrentIndex(option);});
}

void Preview::onClickPattern(const QPoint &point)
{ // 点击图案的某个点,就要切换到photo模式,并依据当前的objective,brand设置photocanvas的绘制策略
    auto objective = toolbar->toolInfo()[ObjectiveField].toString();
    auto brand = toolbar->toolInfo()[BrandField].toString();
    auto manufacturer = toolbar->toolInfo()[ManufacturerField].toString();

    cameramode->changeMode(CameraMode::PhotoMode);

    // 关闭相机,切换stack
    stack->setCurrentWidget(photocanvas);


    // 依据objective,brand等设置不同的策略
    auto manufacturer_idx = getIndexFromFields(manufacturer).toUInt();
    auto brand_idx = getIndexFromFields(brand).toUInt();
    auto objective_idx = getIndexFromFields(objective).toUInt();

    auto size = ViewCircleMapFields[manufacturer_idx][brand_idx][objective_idx];
    LOG<<" manufacturer = "<<manufacturer_idx<<" brand = "<<brand_idx<<"objective = "<<objective_idx<<" size = "<<size;
    if (point != QPoint(-1,-1))
        photocanvas->setStrategy(PreviewPhotoCanvas::InnerCircleRect,size,size);
    else photocanvas->setStrategy(PreviewPhotoCanvas::NoStrategy);
}

void Preview::onManufacturerChanged(int option)
{

}

void Preview::onWellbrandChanged(int option)
{
    auto objective = getIndexFromFields(toolbar->toolInfo()[ObjectiveField].toString()).toUInt();
    auto brand = getIndexFromFields(toolbar->toolInfo()[BrandField].toString()).toUInt();
    auto manufacturer = getIndexFromFields(toolbar->toolInfo()[ManufacturerField].toString()).toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    photocanvas->setStrategy(PreviewPhotoCanvas::InnerCircleRect,size,size);
    Q_ASSERT(option == brand);
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
    auto objective = getIndexFromFields(toolbar->toolInfo()[ObjectiveField].toString()).toUInt();
    auto brand = getIndexFromFields(toolbar->toolInfo()[BrandField].toString()).toUInt();
    auto manufacturer = getIndexFromFields(toolbar->toolInfo()[ManufacturerField].toString()).toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    photocanvas->setStrategy(PreviewPhotoCanvas::InnerCircleRect,size,size);
    Q_ASSERT(option == objective);
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