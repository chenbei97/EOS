/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:28:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:02
 * @FilePath: \EOS\component\src\widgets\label.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "label.h"

Label::Label(QWidget*parent):QLabel(parent)
{
    setFrameShape(QFrame::NoFrame);
}

Label::Label(const QString&text,QWidget*parent) :QLabel(parent)
{
    setText(text);
    setFrameShape(QFrame::NoFrame);
}
