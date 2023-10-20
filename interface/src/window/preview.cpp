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
    auto canvas = new QWidget;
    toolbar = new PreviewTool;
    pattern = new Pattern(2,3);
    pattern->toggleState(Pattern::TickState);

    auto vlay = new QVBoxLayout;
    vlay->addWidget(pattern);
    vlay->addWidget(toolbar);
    auto box = new GroupBox;
    box->setLayout(vlay);
    box->setMaximumWidth(PreviewToolBarMaxWidth);

    auto lay = new QHBoxLayout;
    lay->addWidget(canvas);
    lay->addWidget(box);
    setLayout(lay);

    connect(toolbar,&PreviewTool::wellsizeChanged,this,&Preview::onWellsizeChanged);
    connect(toolbar,&PreviewTool::infoChanged,this,&Preview::onInfoChanged);
}

void Preview::onWellsizeChanged(int option)
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

    AssemblerPointer->assemble(TcpFramePool.frame0x0001,toolinfo);

    LOG<<"request = "<<AssemblerMessage;
}