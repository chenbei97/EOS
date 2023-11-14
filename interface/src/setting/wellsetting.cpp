/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 15:10:15
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-14 15:16:51
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
        singleboardbtn = new QRadioButton(tr("Single Board"));
        multiboardbtn = new QRadioButton(tr("Multi Boards"));
        auto lay = new QHBoxLayout;
        lay->addWidget(singleboardbtn);
        lay->addWidget(multiboardbtn);
        lay->addStretch();
        boardbox->setLayout(lay);
    }
}

void WellSetting::initAttributes()
{
    singleboardbtn->setChecked(true);
}

void WellSetting::initLayout()
{
    auto lay = new QVBoxLayout;

    lay->addWidget(boardbox);
    lay->addStretch();
    setLayout(lay);
    setTitle(tr("Well Setting"));
}