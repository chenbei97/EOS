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
    brbox = new QRadioButton(BRField);
    phbox = new QRadioButton(PHField);
    gfpbox = new QRadioButton(GFPField);
    rfpbox = new QRadioButton(RFPField);
    dapibox = new QRadioButton(DAPIField);

    auto lay = new QHBoxLayout;

    lay->addWidget(brbox);
    lay->addWidget(phbox);
    lay->addWidget(gfpbox);
    lay->addWidget(rfpbox);
    lay->addWidget(dapibox);

    setLayout(lay);
    setTitle(tr("通道"));

    QButtonGroup * group = new QButtonGroup;
    group->addButton(brbox,0);
    group->addButton(phbox,1);
    group->addButton(gfpbox,2);
    group->addButton(rfpbox,3);
    group->addButton(dapibox,4);
    brbox->setChecked(true);

    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ChannelBox::channelChanged);
}

void ChannelBox::onObjectiveSettingChanged(int option)
{
    //LOG<<"option = "<<option;
    if (option == ObjectiveSettingFields.indexOf(NoneField))
        return;

    bool r1 = ((option == ObjectiveSettingFields.indexOf(Bright4xField)) ||
               (option == ObjectiveSettingFields.indexOf(Bright10xField)) ||
               (option == ObjectiveSettingFields.indexOf(Bright20xField)) ||
               (option == ObjectiveSettingFields.indexOf(Bright40xField)));
    if (r1) {
        brbox->setEnabled(true);
        phbox->setEnabled(false);
    } else {
        brbox->setEnabled(false);
        phbox->setEnabled(true);
    }
}

ChannelInfo ChannelBox::channelInfo() const
{
    ChannelInfo m;

    if (brbox->isChecked())
        m[ChannelField] = BRField; // 转换为idx无需在这里,这里需要实际字符串
    else if (phbox->isChecked())
        m[ChannelField] = PHField;
    else if (gfpbox->isChecked())
        m[ChannelField] = GFPField;
    else if (rfpbox->isChecked())
        m[ChannelField] = RFPField;
    else if (dapibox->isChecked())
        m[ChannelField] = DAPIField;

    return m;
}