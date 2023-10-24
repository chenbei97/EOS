/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 09:45:31
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 09:46:00
 * @FilePath: \EOS\interface\src\preview\focustool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "focustool.h"

FocusTool::FocusTool(QWidget *parent) : QWidget(parent)
{
    slider = new Slider;
    slider->setRange(0,500000);
    slider->setPrefix("focus: ");
    slider->setMouseEvent(false);

    step = new SpinBox(true);
    step->setMaximumWidth(100);
    step->setMaximum(250000);
    auto steplabel = new Label("step: ");
    steplabel->setMaximumWidth(100);

    leftbtn = new RoundButton;
    rightbtn = new RoundButton;
    leftbtn->setStrategy(RoundButton::LeftTriangle);
    rightbtn->setStrategy(RoundButton::RightTriangle);

    auto btnlay = new QHBoxLayout;
    btnlay->addWidget(leftbtn);
    btnlay->addStretch();
    btnlay->addWidget(steplabel);
    btnlay->addWidget(step);
    btnlay->addStretch();
    btnlay->addWidget(rightbtn);

    auto lay = new QVBoxLayout;
    lay->addWidget(slider);
    lay->addLayout(btnlay);
    setLayout(lay);

    connect(leftbtn,&PushButton::clicked,this,&FocusTool::subtractFocus);
    connect(rightbtn,&PushButton::clicked,this,&FocusTool::addFocus);
}

void FocusTool::addFocus()
{
    auto currentstep = step->value()*10;
    slider->setValue(currentstep+slider->sliderPosition());
}

void FocusTool::subtractFocus()
{
    auto currentstep = step->value()*10;
    slider->setValue(-currentstep+slider->sliderPosition());
}