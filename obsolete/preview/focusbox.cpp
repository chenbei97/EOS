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
    //connect(slider,&DoubleSlider::sliderMoved,this,&FocusBox::valueChanged);
    connect(slider,&DoubleSlider::sliderReleased,this,&FocusBox::valueChanged); // 改成释放才更改
    connect(autofocusdlg,&AutoFocus::autoFocus,this,&FocusBox::autoFocus);
    connect(qApp, &QCoreApplication::aboutToQuit, [=](){autofocusdlg->close();}); // 防止关了主窗口这个还在
    setTitle(tr(FocusBoxTitle));
}

void FocusBox::onAutoFocus()
{
    autofocusdlg->show();
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
    if (val == 0.0) return;
    slider->addValue(val);
}

void FocusBox::subtractFocus()
{
    auto val = step->value();
    if (val == 0.0) return;
    slider->subtractValue(val);
}

double FocusBox::focus() const
{
    return slider->value();
}

double FocusBox::focusStep() const
{
    return step->value();
}

FocusInfo FocusBox::focusInfo() const
{
    FocusInfo info;
    info[FocusField] = QString::number(focus());
    info[FocusStepField] = QString::number(focusStep());
    return info;
}

void FocusBox::initLayout()
{
    // 1. focus的2个按钮细调
    auto lay1 = new QVBoxLayout;
    lay1->addWidget(topbtn);
    lay1->addStretch();
    lay1->addWidget(bottombtn);

    // 2. "step: " [spinbox]
    auto steplay = new QVBoxLayout;
    steplay->addWidget(new Label(FocusToolStepLabel));
    steplay->addWidget(step);
    steplay->addStretch();
    steplay->setSpacing(0);

    auto lay = new QHBoxLayout;
    lay->addWidget(autofocusbtn);
    lay->addWidget(pattern);
    lay->addWidget(slider);
    lay->addLayout(lay1);
    lay->addLayout(steplay);


    //lay->addStretch();
    lay->setSpacing(FocusBoxSpacing);
    setLayout(lay);
}

void FocusBox::initObjects()
{
    pattern = new TriangleMove; // 懒得删代码了
    autofocusbtn = new PushButton(tr(AutoFocusField));
    autofocusdlg = new AutoFocus();

    slider = new DoubleSlider;
    step = new SpinBox(true);

    topbtn = new RoundButton;
    bottombtn = new RoundButton;
}

void FocusBox::initAttributes()
{
    pattern->hide(); // 不需要在这里调镜头位置了,但是懒得去掉代码直接隐藏就行了

    slider->setDirection(Qt::Vertical);
    slider->setRange(0,FocusToolFocusMaxVal);
    slider->setScaleFactor(FocusToolScaleVal);
    slider->setMouseEvent(true);
    slider->setPrefixVisible(false);

    step->setValue(0.000);
    step->setMaximumWidth(FocusToolStepSpinMaxWidth);
    step->setSingleStep(FocusToolStepVal);
    step->setDecimals(FocusToolStepDecimal);
    step->setMaximum(FocusToolStepMaxVal);

    topbtn->setStrategy(RoundButton::TopTriangle);
    bottombtn->setStrategy(RoundButton::BottomTriangle);
}

