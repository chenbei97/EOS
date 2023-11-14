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
    initObjects();
    initAttributes();
    initLayout();

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

void FocusBox::initLayout()
{
    // "step" [spinbox]
    auto steplay = new QHBoxLayout;
    steplay->addWidget(new Label(FocusToolStepLabel));
    steplay->addWidget(step);
    steplay->addStretch();

    // 2. focus的2个按钮细调
    auto lay1 = new QVBoxLayout;
    lay1->addWidget(topbtn);
    lay1->addStretch();
    lay1->addWidget(bottombtn);

    // 3. 自动聚焦按钮
    auto holelay = new QHBoxLayout;//自动聚焦跳过的孔数和视野数
    holelay->addWidget(new Label(FocusSkipHolesLabelField));
    holelay->addWidget(skipholes);
    holelay->addStretch();

    auto viewlay = new QHBoxLayout;
    viewlay->addWidget(new Label(FocusSkipViewsLabelField));
    viewlay->addWidget(skipviews);
    viewlay->addStretch();

    auto lay2 = new QVBoxLayout;
    lay2->addStretch();
    lay2->addLayout(holelay);
    lay2->addLayout(viewlay);
    lay2->addWidget(autofocusbtn);

    auto lay = new QHBoxLayout;
    lay->addWidget(pattern);
    lay->addWidget(slider);
    lay->addLayout(lay1);
    lay->addLayout(steplay);
    lay->addLayout(lay2);

    lay->addStretch();
    lay->setSpacing(FocusBoxSpacing);

    setLayout(lay);
}

void FocusBox::initObjects()
{
    pattern = new TriangleMove;
    autofocusbtn = new PushButton(tr("auto focus"));

    slider = new DoubleSlider;
    step = new SpinBox(true);

    topbtn = new RoundButton;
    bottombtn = new RoundButton;

    skipholes = new SpinBox;
    skipviews = new SpinBox;
}

void FocusBox::initAttributes()
{
    pattern->hide(); // 不需要在这里调镜头位置了,但是懒得去掉代码直接隐藏就行了

    slider->setDirection(Qt::Vertical);
    slider->setRange(0,FocusToolFocusMaxVal);
    slider->setPrefix("");
    slider->setMouseEvent(true);

    step->setMaximumWidth(FocusToolStepSpinMaxWidth);
    step->setMaximum(FocusToolStepMaxVal);

    topbtn->setStrategy(RoundButton::TopTriangle);
    bottombtn->setStrategy(RoundButton::BottomTriangle);
}
