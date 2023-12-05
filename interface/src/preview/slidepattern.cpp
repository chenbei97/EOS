/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 16:50:16
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 16:50:56
 * @FilePath: \EOS\interface\src\preview\slidepattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "slidepattern.h"


void SlidePattern::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(DefaultPainterPenWidth * 2);
    painter.setPen(pen);

    painter.drawRect(0,0,width(),height());
}

SlidePattern::SlidePattern(QWidget*parent): QWidget(parent)
{

}