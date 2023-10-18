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
    slider = new QSlider(Qt::Horizontal);
    suffix = new Label;
    prefix = new Label;

    auto hlay = new QHBoxLayout;
    hlay->addWidget(prefix);
    hlay->addWidget(slider);
    hlay->addWidget(suffix);
    setLayout(hlay);

    connect(slider,&QSlider::sliderMoved,this,&Slider::onSliderChanged);
    setMinimumHeight(50);
}

Slider::Slider(const QString &p, const QString &s, int min, int max, QWidget *parent) :QWidget(parent)
{
    slider = new QSlider(Qt::Horizontal);
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
    setLayout(hlay);

    connect(slider,&QSlider::sliderMoved,this,&Slider::onSliderChanged);
    setMinimumHeight(50);
}

void Slider::onSliderChanged(int val)
{
    auto text = QString("%1 %2").arg(val).arg(suffixtext);
    suffix->setText(text);
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
