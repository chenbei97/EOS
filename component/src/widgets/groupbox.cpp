/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:22:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:59
 * @FilePath: \EOS\component\src\widgets\groupbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "groupbox.h"

GroupBox::GroupBox(QWidget*parent):QGroupBox(parent)
{
}

GroupBox::GroupBox(const QString&title,QWidget*parent):QGroupBox(parent)
{
    setTitle(title);
}

GroupBox::GroupBox(QLayout*lay,QWidget*parent):QGroupBox(parent)
{
    setLayout(lay);
}

GroupBox::GroupBox(const QString&title,QLayout*lay,QWidget*parent):QGroupBox(parent)
{
    setTitle(title);
    setLayout(lay);
}

void GroupBox::setID(int id)
{ // 用于通知者模式使用
    this->id = id;
}

int GroupBox::getID() const
{// 用于通知者模式使用
    return id;
}