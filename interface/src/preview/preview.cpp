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
    photocanvas = new QWidget;
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
    stack->addWidget(photocanvas);
    stack->addWidget(livecanvas);
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
    connect(toolbar,&PreviewTool::infoChanged,this,&Preview::onInfoChanged);
}

void Preview::onManufacturerChanged(int option)
{
    //LOG<<"wellsize option = "<<option;
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