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
}

void Sliderx::mousePressEvent(QMouseEvent *event)
{ // 为了支持点击滑动条也能更改值
    QSlider::mousePressEvent(event); // 需要父类
    if (!enableMouse)
        event->ignore();
    else {
        double per = 0.0;
        if (orientation() == Qt::Horizontal) {
            int currentX = event->pos().x(); // 鼠标点击滑动条的位置
            per = currentX * 1.0 / width();//获取当前点击的位置占整个slider的百分比
        } else {
            int currentY = event->pos().y(); // 鼠标点击滑动条的位置
            per = 1.0 - currentY * 1.0 / height();//获取当前点击的位置占整个slider的百分比
        }

        // (val-min)/(max-min) = per => val = per*(max-min)+min
        int value = per*(maximum() - minimum()) + minimum();

        setValue(value);
        setSliderPosition(value);
        emit valueChanged(value);
        emit sliderMoved(value); // 这个信号也要发送
        emit sliderPressed();
    }
}

void Sliderx::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event); // 需要
    emit sliderReleased();
}

void Sliderx::setMouseEvent(bool enabled)
{
    enableMouse = enabled;
}

void Sliderx::wheelEvent(QWheelEvent *event)
{
    //QSlider::wheelEvent(event); // 不要使用父类,否则总是会多转一下
    QPoint angle = event->angleDelta();
    //LOG<<angle<<event->delta();
    if (angle.y() > 0) { // 鼠标前转,数值增加
        if (sliderPosition()+singleStep() <= maximum())
            setValue(sliderPosition()+singleStep());
        else setValue(maximum());
    }
    else { // 鼠标后转,数值减少
        if (sliderPosition() - singleStep() >= minimum())
            setValue(sliderPosition()-singleStep()); // 0还可以设置
        else setValue(minimum());
    }
    emit sliderMoved(sliderPosition()); // 这个信号也要发送
    emit sliderPressed();
    emit sliderReleased();
}

void Sliderx::keyPressEvent(QKeyEvent *event)
{
    if (orientation() == Qt::Horizontal) {
        if (event->key() == Qt::Key_Right) {
            if (sliderPosition()+singleStep() <= maximum())
                setValue(sliderPosition()+singleStep());
            else setValue(maximum());
        } else if (event->key() == Qt::Key_Left) {
            if (sliderPosition() - singleStep() >= minimum())
                setValue(sliderPosition()-singleStep());
            else setValue(minimum());
        }
    } else if (orientation() == Qt::Vertical) {
        if (event->key() == Qt::Key_Up) {
            if (sliderPosition()+singleStep() <= maximum())
                setValue(sliderPosition()+singleStep());
            else setValue(maximum());
        } else if (event->key() == Qt::Key_Down) {
            if (sliderPosition() - singleStep() >= minimum())
                setValue(sliderPosition()-singleStep());
            else setValue(minimum());
        }
    }
    emit sliderMoved(sliderPosition()); // 这个信号也要发送
    emit sliderPressed();
    emit sliderReleased();
    //QSlider::keyPressEvent(event);// 不要使用父类,否则总是会多增加一下,而且水平点击上下也会动
    event->accept();
}