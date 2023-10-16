/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:44:40
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:10
 * @FilePath: \EOS\component\src\widgets\progressbar.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "progressbar.h"

ProgressBar::ProgressBar(QWidget * parent) :QProgressBar(parent)
{
    setRange(0,100);
    setValue(50);
    setAlignment(Qt::AlignCenter);
    setStyleSheet(ProgressBarStyle);
    setMinimumHeight(ProgressBarMinHeight);
}