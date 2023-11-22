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
    videoformat = new ComboBox(VideoFormatFields);
    imageformat = new ComboBox(ImageFormatFields);

    auto hlay1 = new QHBoxLayout;
    hlay1->addWidget(zstack);
    hlay1->addWidget(stitchimage);
    hlay1->setSpacing(30);
    hlay1->addStretch();

    auto hlay2 = new QHBoxLayout;
    hlay2->addWidget(new Label(ImageFormatLabelField));
    hlay2->addWidget(imageformat);
    hlay2->addWidget(new Label(VideoFormatLabelField));
    hlay2->addWidget(videoformat);
    hlay1->setSpacing(30);
    hlay2->addStretch();

    auto vlay = new QVBoxLayout;
    vlay->addLayout(hlay1);
    //vlay->addLayout(hlay2); // 这2个不需要,懒得删代码隐藏即可

    setLayout(vlay);
    setTitle(tr("Other"));

    connect(zstack,&CheckBox::checkedChanged,this,&ZStackBox::zstackChanged);
    connect(stitchimage,&CheckBox::checkedChanged,this,&ZStackBox::stitchChanged);
}

void ZStackBox::importExperConfig(bool zstack, bool stitch)
{
    this->zstack->setChecked(zstack);
    this->stitchimage->setChecked(stitch);
}

ZStackInfo ZStackBox::zstackInfo() const
{
    ZStackInfo info;

    info[ZStackField] = QString::number((int)zstack->isChecked());
    info[StitchField] = QString::number((int)stitchimage->isChecked());

    return info;
}
