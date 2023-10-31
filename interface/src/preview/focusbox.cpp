/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 09:45:31
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 09:46:00
 * @FilePath: \EOS\interface\src\preview\focustool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "focusbox.h"

FocusBox::FocusBox(QWidget *parent) : GroupBox(parent)
{
    autofocusbtn = new PushButton(tr("自动聚焦"));

    slider = new DoubleSlider;
    slider->setRange(0,FocusToolFocusMaxVal);
    slider->setPrefix(FocusToolFocusLabel);
    slider->setMouseEvent(false);

    step = new SpinBox(true);
    step->setMaximumWidth(FocusToolStepSpinMaxWidth);
    step->setMaximum(FocusToolStepMaxVal);
    auto steplabel = new Label(FocusToolStepLabel);
    steplabel->setMaximumWidth(FocusToolStepSpinMaxWidth);

    leftbtn = new RoundButton;
    rightbtn = new RoundButton;
    leftbtn->setStrategy(RoundButton::LeftTriangle);
    rightbtn->setStrategy(RoundButton::RightTriangle);

    auto btnlay = new QHBoxLayout;
    btnlay->addSpacing(FocusBoxButtonMargin);
    btnlay->addWidget(leftbtn);
    btnlay->addStretch();
    btnlay->addWidget(steplabel);
    btnlay->addWidget(step);
    btnlay->addStretch();
    btnlay->addWidget(rightbtn);
    btnlay->addSpacing(FocusBoxButtonMargin);
    btnlay->addWidget(autofocusbtn);

    auto lay = new QVBoxLayout;
    lay->addWidget(slider);
    lay->addLayout(btnlay);

    setLayout(lay);

    connect(leftbtn,&PushButton::clicked,this,&FocusBox::subtractFocus);
    connect(rightbtn,&PushButton::clicked,this,&FocusBox::addFocus);
    connect(autofocusbtn,&PushButton::clicked,this,&FocusBox::onAutoFocus);
    setTitle(tr("焦距"));
}

void FocusBox::onAutoFocus()
{

}

void FocusBox::setEnabled(bool enabled)
{
    slider->setEnabled(enabled);
    leftbtn->setEnabled(enabled);
    rightbtn->setEnabled(enabled);
    step->setEnabled(enabled);
}

void FocusBox::addFocus()
{
    auto val = step->value();
    slider->addValue(step->value());
}

void FocusBox::subtractFocus()
{
    slider->subtractValue(step->value());
}

int FocusBox::focus() const
{
    return slider->value();
}