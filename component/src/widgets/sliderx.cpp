/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-20 13:50:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-20 13:51:29
 * @FilePath: \EOS\component\src\widgets\sliderx.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "sliderx.h"

Sliderx::Sliderx(QWidget *parent) : QSlider(parent)
{
    setSingleStep(1);
    setMinimum(0);
    setMaximum(100);
    setOrientation(Qt::Horizontal);
}

Sliderx::Sliderx(Qt::Orientation orientation, QWidget *parent) : QSlider(parent)
{
    setSingleStep(1);
    setMinimum(0);
    setMaximum(100);
    setOrientation(orientation);
//    setStyleSheet("QSlider{border:1px solid #5c5c5c;border-radius:3px;}"
//                  "QSlider::handle:horizontal { background-color:#8A5933;}");
}

void Sliderx::mousePressEvent(QMouseEvent *event)
{
    if (!enableMouse)
        event->ignore();
    else {
        int currentX = event->pos().x(); // 鼠标点击滑动条的位置

        double per = currentX *1.0 /width();//获取当前点击的位置占整个slider的百分比

        // (val-min)/(max-min) = per => val = per*(max-min)+min
        int value = per*(maximum() - minimum()) + minimum();

        setValue(value);
        setSliderPosition(value);
        emit valueChanged(value);
        emit sliderMoved(value); // 这个信号也要发送

        QSlider::mousePressEvent(event);
    }
}

void Sliderx::setMouseEvent(bool enabled)
{
    enableMouse = enabled;
}