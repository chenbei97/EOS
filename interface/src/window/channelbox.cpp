/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 10:19:59
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 10:20:26
 * @FilePath: \EOS\interface\src\window\channelbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "channelbox.h"

ChannelBox::ChannelBox(QWidget*parent): GroupBox(parent)
{
    phbox = new CheckBox("PH");
    gfpbox = new CheckBox("GFP");
    rfpbox = new CheckBox("RFP");
    dapibox = new CheckBox("DAPI");

    auto lay = new QHBoxLayout;

    lay->addWidget(new Label("channel: "));
    lay->addWidget(phbox);
    lay->addWidget(gfpbox);
    lay->addWidget(rfpbox);
    lay->addWidget(dapibox);

    setLayout(lay);

    connect(phbox,&CheckBox::checkedChanged,this,&ChannelBox::phSelected);
    connect(gfpbox,&CheckBox::checkedChanged,this,&ChannelBox::gfpSelected);
    connect(rfpbox,&CheckBox::checkedChanged,this,&ChannelBox::rfpSelected);
    connect(dapibox,&CheckBox::checkedChanged,this,&ChannelBox::dapiSelected);
}