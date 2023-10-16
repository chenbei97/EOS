/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:07:46
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:54
 * @FilePath: \EOS\component\src\widgets\Combobox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "combobox.h"

ComboBox::ComboBox(QWidget*parent):QComboBox(parent)
{
    setEditable(false);
}

ComboBox::ComboBox(const QString&text,QWidget*parent):QComboBox(parent)
{
    addItem(text);
    setEditable(false);
}

ComboBox::ComboBox(const QStringList&texts,QWidget*parent):QComboBox(parent)
{
    addItems(texts);
    setEditable(false);
}