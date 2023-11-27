/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 10:19:59
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-01 16:14:22
 * @FilePath: \EOS\interface\src\preview\channelbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "channelbox.h"

ChannelBox::ChannelBox(QWidget*parent): GroupBox(parent)
{
    brbox = new QRadioButton(BRField);
    phbox = new QRadioButton(PHField);
    gfpbox = new QRadioButton(GFPField);
    rfpbox = new QRadioButton(RFPField);
    dapibox = new QRadioButton(DAPIField);

    // objectivesetting,默认是br4x,故objectivebox默认是br4x,故默认brbox是启用,phbox禁用
    brbox->setEnabled(true);
    phbox->setEnabled(false);

    auto lay = new QHBoxLayout;

    lay->addWidget(brbox);
    lay->addWidget(phbox);
    lay->addWidget(gfpbox);
    lay->addWidget(rfpbox);
    lay->addWidget(dapibox);

    setLayout(lay);
    setTitle(tr(ChannelBoxTitle));

    QButtonGroup * group = new QButtonGroup;
    group->addButton(brbox,0);
    group->addButton(phbox,1);
    group->addButton(gfpbox,2);
    group->addButton(rfpbox,3);
    group->addButton(dapibox,4);
    brbox->setChecked(true);

    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ChannelBox::channelChanged);
}

void ChannelBox::disableChannel(const QString &obj)
{ // obj = br4x,ph4x
    if (obj.contains(ObjectiveBR)) {
        phbox->setEnabled(false);
        brbox->setEnabled(true);
    } else if (obj.contains(ObjectivePH)) {
        phbox->setEnabled(true);
        brbox->setEnabled(false);
    } else {
        // 可能是无镜头 none 对通道的勾选没有影响
    }
}

ChannelInfo ChannelBox::channelInfo() const
{
    ChannelInfo m;

    if (brbox->isChecked())
        m[CurrentChannelField] = BRField; // 转换为idx无需在这里,这里需要实际字符串
    else if (phbox->isChecked())
        m[CurrentChannelField] = PHField;
    else if (gfpbox->isChecked())
        m[CurrentChannelField] = GFPField;
    else if (rfpbox->isChecked())
        m[CurrentChannelField] = RFPField;
    else if (dapibox->isChecked())
        m[CurrentChannelField] = DAPIField;

    return m;
}