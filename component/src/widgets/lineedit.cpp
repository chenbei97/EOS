/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:35:13
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:05
 * @FilePath: \EOS\component\src\widgets\lineedit.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "lineedit.h"

LineEdit::LineEdit(QWidget*parent):QLineEdit(parent)
{

}

LineEdit::LineEdit(const QString&text,QWidget*parent):QLineEdit(parent)
{
    setText(text);
}
