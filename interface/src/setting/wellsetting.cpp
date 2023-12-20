/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 15:10:15
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:12:03
 * @FilePath: \EOS\interface\src\setting\wellsetting.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "wellsetting.h"

WellSetting::WellSetting(QWidget *parent) : GroupBox(parent)
{
    initObjects();
    initAttributes();
    initLayout();
}

void WellSetting::initObjects()
{
    {
        boardbox = new GroupBox(tr("Well Board"));
        radioGroup = new RadioGroup;
        auto lay = new QHBoxLayout;
        lay->addWidget(radioGroup);
        lay->addStretch();
        boardbox->setLayout(lay);
    }
}

void WellSetting::initAttributes()
{
    radioGroup->setText(1,tr("Single Board"));
    radioGroup->setText(2,tr("Multi Boards"));
    radioGroup->setChecked(1,true);
}

void WellSetting::initLayout()
{
    auto lay = new QVBoxLayout;

    lay->addWidget(boardbox);
    lay->addStretch();
    setLayout(lay);
    setTitle(tr("Well Setting"));
}