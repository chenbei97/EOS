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

}


PushButton::PushButton(const QString&text,QWidget*parent):QPushButton(parent)
{
    setText(text);
}


