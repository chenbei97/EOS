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
    cameraExposureSlider = new Slider("exposure: ","ms",0,15000);
    cameraGainSlider = new Slider("gain: ","%",100,5000);
    cameraBrightSlider = new Slider("bright: ","",0,100);
    label->resize(label->width(),cameraBrightSlider->height());

    auto vlay = new QVBoxLayout;
    vlay->addWidget(label);
    vlay->addWidget(cameraExposureSlider);
    vlay->addWidget(cameraGainSlider);
    vlay->addWidget(cameraBrightSlider);
    //vlay->setMargin(0);
    //vlay->setSpacing(0);

    setLayout(vlay);

    connect(cameraExposureSlider,&Slider::valueChanged,this,&CameraTool::exposureChanged);
    connect(cameraGainSlider,&Slider::valueChanged,this,&CameraTool::gainChanged);
    connect(cameraBrightSlider,&Slider::valueChanged,this,&CameraTool::brightChanged);

    //setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
}

void CameraTool::setTitle(const QString &title)
{
    label->show();
    label->setText(title);
}

//QSize CameraTool::sizeHint() const
//{
//    return QSize(SliderSizeHintWidth,SliderSizeHintHeight*3);
//}

//QSize CameraTool::minimumSizeHint() const
//{ // 要在布局起作用需要重载本函数,sizeHint不起作用
//    return QSize(SliderSizeHintWidth,SliderSizeHintHeight*4);
//}