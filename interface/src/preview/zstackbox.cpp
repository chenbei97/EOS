/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 13:43:21
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-01 16:55:34
 * @FilePath: \EOS\interface\src\preview\zstackbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "zstackbox.h"

ZStackBox::ZStackBox(QWidget *parent) : GroupBox(parent)
{
    zstack = new CheckBox("z_stack");
    stitchimage = new CheckBox("stitch_image");

    auto hlay = new QHBoxLayout;
    hlay->addWidget(zstack);
    hlay->addWidget(stitchimage);
    hlay->setSpacing(30);
    hlay->addStretch();

    setLayout(hlay);
    setTitle(tr("其它"));

    connect(zstack,&CheckBox::checkedChanged,this,&ZStackBox::zstackChanged);
    connect(stitchimage,&CheckBox::checkedChanged,this,&ZStackBox::stitchChanged);
}

ZStackInfo ZStackBox::zstackInfo() const
{
    ZStackInfo info;

    info[ZStackField] = QString::number((int)zstack->isChecked());
    info[StitchField] = QString::number((int)stitchimage->isChecked());

    return info;
}
