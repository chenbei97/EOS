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
    resetBackGroundColor();
}


PushButton::PushButton(const QString&text,QWidget*parent):QPushButton(parent)
{
    checked = false;
    setAutoFillBackground(true);
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
    auto sheet = tr("QPushButton{border:2px solid #3282F6;border-radius: 5px;min-height: 30px;background-color: %1;}\n"
                 "QPushButton:default {border-color:#FF095E;}\n"
                 "QPushButton:flat {border:2px solid #FFFD55;border:none;}\n"
                 "QPushButton:hover {background-color:#A93700;}"
                 "QPushButton:disabled {background-color:gray;color:black;}");
    setStyleSheet(sheet.arg(color.name()));
    checked = true;
}

void PushButton::resetBackGroundColor()
{
//    auto pal = palette();
//    pal.setColor(QPalette::Button,"#F0F0F0");
//    setPalette(pal);
    //setStyleSheet("background-color:#F0F0F0");
    auto sheet = tr("QPushButton{border: 2px solid #3282F6;border-radius: 5px;min-height: 30px;}\n"
                    //"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 #F09B59,stop: 1 #FF8E55);}\n"
                    "QPushButton:default {border-color:#FF095E;}\n"
                    "QPushButton:flat {border:2px solid #FFFD55;border:none;}\n"
                    "QPushButton:hover {background-color:#A93700;}"
                    "QPushButton:disabled {background-color:gray;color:black;}"
                    );
    setStyleSheet(sheet);
    checked = false;
}

void PushButton::setID(int id)
{
    this->id = id;
}

int PushButton::getID() const
{
    return id;
}
