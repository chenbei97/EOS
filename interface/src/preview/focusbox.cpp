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
    pattern = new TriangleMove;
    pattern->hide(); // 不在这里调镜头位置了,懒得去掉代码直接隐藏就行了
    autofocusbtn = new PushButton(tr("auto focus"));

    slider = new DoubleSlider;
    slider->setDirection(Qt::Vertical);
    slider->setRange(0,FocusToolFocusMaxVal);
    slider->setPrefix("");
    slider->setMouseEvent(true);

    step = new SpinBox(true);
    step->setMaximumWidth(FocusToolStepSpinMaxWidth);
    step->setMaximum(FocusToolStepMaxVal);
    auto steplay = new QHBoxLayout;
    steplay->addWidget(new Label(FocusToolStepLabel));
    steplay->addWidget(step);

    topbtn = new RoundButton;
    bottombtn = new RoundButton;
    topbtn->setStrategy(RoundButton::TopTriangle);
    bottombtn->setStrategy(RoundButton::BottomTriangle);

    auto lay1 = new QVBoxLayout;
    lay1->addWidget(topbtn);
    lay1->addStretch();
    lay1->addWidget(bottombtn);

    auto lay2 = new QVBoxLayout;
    lay2->addLayout(steplay);
    lay2->addStretch();
    lay2->addWidget(autofocusbtn);
    lay2->setSpacing(0);

    auto lay = new QHBoxLayout;
    //lay->addWidget(pattern);
    lay->addWidget(slider);
    lay->addLayout(lay1);
    lay->addLayout(lay2);

    lay->addStretch();
    lay->setSpacing(FocusBoxSpacing);

    setLayout(lay);

    connect(topbtn,&PushButton::clicked,this,&FocusBox::addFocus);
    connect(bottombtn,&PushButton::clicked,this,&FocusBox::subtractFocus);
    connect(autofocusbtn,&PushButton::clicked,this,&FocusBox::onAutoFocus);
    connect(pattern,&TriangleMove::leftTriangleClicked,this,&FocusBox::leftMove);
    connect(pattern,&TriangleMove::rightTriangleClicked,this,&FocusBox::rightMove);
    connect(pattern,&TriangleMove::topTriangleClicked,this,&FocusBox::topMove);
    connect(pattern,&TriangleMove::bottomTriangleClicked,this,&FocusBox::bottomMove);
    connect(pattern,&TriangleMove::triangleClicked,this,&FocusBox::directionMove);
    setTitle(tr("Focus"));
}

void FocusBox::onAutoFocus()
{

}

void FocusBox::importExperConfig(double focus, double s)
{
    slider->setValue(focus);
    step->setValue(s);
}

void FocusBox::setEnabled(bool enabled)
{
    slider->setEnabled(enabled);
    topbtn->setEnabled(enabled);
    bottombtn->setEnabled(enabled);
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

double FocusBox::focus() const
{
    return slider->value();
}

double FocusBox::focusStep() const
{
    return step->value();
}