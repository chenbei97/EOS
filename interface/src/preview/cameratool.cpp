/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 08:52:28
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 09:01:54
 * @FilePath: \EOS\interface\src\window\cameratool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "cameratool.h"

CameraTool::CameraTool(QWidget*parent): QWidget(parent)
{
    label = new Label();
    label->hide();
    expslider = new Slider(ExposureFieldLabel,ExposureFieldUnit,ExposureLowLimit,ExposureUpLimit);
    gainslider = new Slider(GainFieldLabel,GainFieldUnit,GainLowLimit,GainUpLimit);
    brightslider = new Slider(BrightFieldLabel,"",BrightLowLimit,BrightUpLimit);
    expslider->setSingleStep(1);

    auto vlay = new QVBoxLayout;
    vlay->addWidget(label);
    vlay->addWidget(expslider);
    vlay->addWidget(gainslider);
    vlay->addWidget(brightslider);
    //vlay->setMargin(0);
    //vlay->setSpacing(0);

    setLayout(vlay);

    connect(expslider,&Slider::valueChanged,this,&CameraTool::exposureChanged);
    connect(gainslider,&Slider::valueChanged,this,&CameraTool::gainChanged);
    //connect(brightslider,&Slider::valueChanged,this,&CameraTool::brightChanged);
    connect(brightslider,&Slider::sliderReleased,this,&CameraTool::brightChanged);
    //setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
}

void CameraTool::setEnabled(int tool, bool enable)
{ // 对3个滑动条之一进行设置使能
    switch (tool) {
        case 0:
            expslider->setEnabled(enable);
            break;
        case 1:
            gainslider->setEnabled(enable);
            break;
        case 2:
            brightslider->setEnabled(enable);
            break;
    }
}

void CameraTool::setTitle(const QString &title)
{
    label->show();
    label->setText(title);
}

void CameraTool::setExposure(int exposure)
{
    expslider->setValue(exposure);
}

QString CameraTool::exposure() const
{
    return expslider->value();
}

QString CameraTool::gain() const
{
    return gainslider->value();
}

void CameraTool::setGain(int gain)
{
    gainslider->setValue(gain);
}

QString CameraTool::bright() const
{
    return brightslider->value();
}

void CameraTool::setBright(int bright)
{
    brightslider->setValue(bright);
}

//QSize CameraTool::sizeHint() const
//{
//    return QSize(SliderSizeHintWidth,SliderSizeHintHeight*3);
//}

//QSize CameraTool::minimumSizeHint() const
//{ // 要在布局起作用需要重载本函数,sizeHint不起作用
//    return QSize(SliderSizeHintWidth,SliderSizeHintHeight*4);
//}