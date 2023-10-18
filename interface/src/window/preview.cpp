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
    pattern = new Pattern(16,24);
    pattern->toggleState(Pattern::TickState);

    auto vlay = new QVBoxLayout;
    vlay->addWidget(pattern);
    vlay->addWidget(toolbar);
    auto box = new GroupBox;
    box->setLayout(vlay);
    box->setMaximumWidth(800);

    auto lay = new QHBoxLayout;
    lay->addWidget(canvas);
    lay->addWidget(box);
    setLayout(lay);
}