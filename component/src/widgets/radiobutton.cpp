/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-20 09:49:16
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 09:49:59
 * @FilePath: \EOS\component\src\widgets\radiobutton.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "radiobutton.h"

RadioButton::RadioButton(QWidget *parent) : QRadioButton(parent)
{

}

RadioButton::RadioButton(const QString&text,QWidget *parent) : QRadioButton(text,parent)
{

}