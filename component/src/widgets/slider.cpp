/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:24:21
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:01:33
 * @FilePath: \EOS\component\src\widgets\slider.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "slider.h"

Slider::Slider(QWidget *parent):QWidget(parent)
{
    slider = new Sliderx(Qt::Horizontal);
    suffix = new Label;
    prefix = new Label;

    auto hlay = new QHBoxLayout;
    hlay->addWidget(prefix);
    hlay->addWidget(slider);
    hlay->addWidget(suffix);
    hlay->setMargin(0);
    hlay->setSpacing(0);
    setLayout(hlay);

    connect(slider,&QSlider::sliderMoved,this,&Slider::onSliderChanged);
    connect(slider,&QSlider::sliderMoved,this,&Slider::valueChanged);


    //setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
}

Slider::Slider(const QString &p, const QString &s, int min, int max, QWidget *parent) :QWidget(parent)
{
    slider = new Sliderx(Qt::Horizontal);
    suffix = new Label;
    prefix = new Label;

    prefix->setText(p);
    suffix->setText(s);
    slider->setRange(min,max);
    suffixtext = s;

    auto hlay = new QHBoxLayout;
    hlay->addWidget(prefix);
    hlay->addWidget(slider);
    hlay->addWidget(suffix);
    hlay->setMargin(0);
    hlay->setSpacing(0);
    setLayout(hlay);

    connect(slider,&QSlider::sliderMoved,this,&Slider::onSliderChanged);
    connect(slider,&QSlider::sliderMoved,this,&Slider::valueChanged);

    //setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
}

void Slider::onSliderChanged(int val)
{
    //LOG<<"slider->value() = "<<slider->value()<<" val = "<<val; // 会发现2个值不相同,value()直接获取总是慢一拍
    auto text = QString("%1 %2").arg(val).arg(suffixtext);
    suffix->setText(text);
    currentValue = val;
}

void Slider::setSingleStep(int step)
{
    slider->setSingleStep(step);
}

void Slider::setRange(int min,int max)
{
    slider->setRange(min,max);
}

void Slider::setPrefix(const QString& p)
{
    prefix->setText(p);
}

void Slider::setSuffix(const QString& s)
{
    suffixtext = s;
    suffix->setText(s);
}

QString Slider::value() const
{
    return QString::number(currentValue);
    //return QString::number(slider->value()); // 总是慢一拍
}

void Slider::setValue(int value)
{
    currentValue = value;
    slider->setValue(value);
    slider->sliderMoved(value);
}

//QSize Slider::sizeHint() const
//{
//    return QSize(SliderSizeHintWidth,SliderSizeHintHeight);
//}