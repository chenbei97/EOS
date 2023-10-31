/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 14:14:01
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 14:28:01
 * @FilePath: \EOS\interface\src\preview\lensbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "lensbox.h"

LensBox::LensBox(QWidget*parent): GroupBox(parent)
{
    pattern = new TriangleMove;

    auto lay = new QHBoxLayout;
    lay->addStretch();
    lay->addWidget(pattern);
    lay->addStretch();

    setLayout(lay);
    setTitle(tr("镜头"));

    connect(pattern,&TriangleMove::leftTriangleClicked,this,&LensBox::leftMove);
    connect(pattern,&TriangleMove::rightTriangleClicked,this,&LensBox::rightMove);
    connect(pattern,&TriangleMove::topTriangleClicked,this,&LensBox::topMove);
    connect(pattern,&TriangleMove::bottomTriangleClicked,this,&LensBox::bottomMove);
}

LensInfo LensBox::lensInfo() const
{
    return {};
}