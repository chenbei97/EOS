/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:41:59
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:17
 * @FilePath: \EOS\component\src\widgets\pushbutton.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "pushbutton.h"

PushButton::PushButton(QWidget*parent):QPushButton(parent)
{
    checked = false;
    setAutoFillBackground(true);
    //setFlat(true);
    resetBackGroundColor();
}


PushButton::PushButton(const QString&text,QWidget*parent):QPushButton(parent)
{
    checked = false;
    setAutoFillBackground(true);
    //setFlat(true);
    setText(text);
    resetBackGroundColor();
}

bool PushButton::isChecked() const
{
    return checked;
}

void PushButton::setBackGroundColor(QCColor color)
{
//    auto pal = palette();
//    pal.setColor(QPalette::Button,color);
//    setPalette(pal);
    setStyleSheet("background-color:#FFF200");
    checked = true;
}

void PushButton::resetBackGroundColor()
{
//    auto pal = palette();
//    pal.setColor(QPalette::Button,"#F0F0F0");
//    setPalette(pal);
    setStyleSheet("background-color:#F0F0F0");
    checked = false;
}

