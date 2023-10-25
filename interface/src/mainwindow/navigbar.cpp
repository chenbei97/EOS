/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:42:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 10:43:09
 * @FilePath: \EOS\interface\src\navigbar.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "include/mainwindow/navigbar.h"

NavigBar::NavigBar(QWidget*parent): QWidget(parent)
{
    foreach(auto text, NavigBarFields)
        pixwidths << (NavigBarWidth-PainterMetric.width(text))/2;

    //setFixedHeight(NavigBarHeight);
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum); // 代替setFixedSize
}

void NavigBar::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    QPainterPath path;
    for(int i = 0; i < NavigBarFieldsCount; ++i) {
        path.clear(); // 起始坐标(0,0),向右移动的距离是gap+width;text的起始坐标是(pixwidths[i],NavigBarFontHeight),高度不变
        path.addRect(0+NavigBarMoveDistance*i,0,NavigBarWidth,NavigBarHeight);
        path.addText(pixwidths[i]+NavigBarMoveDistance*i,NavigBarFontHeight,PainterFont,NavigBarFields[i]);
        painter.fillPath(path,NavigBarColor);
    }

    // 没有文字的部分,为了好看继续填充矩形,为了调整窗口大小时自动绘制,计算宽度减去已经绘制的宽度得到还要继续填充几个矩形
    auto count = (width() - NavigBarFieldsCount*NavigBarMoveDistance)/(NavigBarMoveDistance);
    for(int i = NavigBarFieldsCount;i <= count+NavigBarFieldsCount; ++i) {
        path.clear();
        path.addRect(0+NavigBarMoveDistance*i,0,NavigBarWidth,NavigBarHeight);
        painter.fillPath(path,NavigBarColor);
    }

    event->accept();
}

void NavigBar::mousePressEvent(QMouseEvent *event)
{
    auto pos = event->pos();
    auto rects = getRects();
    
    for(int c = 0; c < rects.count(); ++c) {
        if (rects[c].contains(pos)) {
            //LOG<<"click toolbar index = "<<c;
            switch (c) {
                case 1: emit mainClicked();break;
                case 2: emit previewClicked();break;
                case 3: emit protocolClicked();break;
                case 4: emit dataClicked();break;
                case 5: emit analysisClicked();break;
                default:break;
            }
        }
    }
    event->accept();
}

QRectVector NavigBar::getRects() const
{ // 获取所有文字部分的小正方形区域
    QRectVector vec;
    for(int i = 0; i < NavigBarFieldsCount; ++i) {
        auto x = 0 + NavigBarMoveDistance*i;
        vec.append(QRect(x,0,NavigBarWidth,NavigBarHeight));
    }
    return vec;
}

QSize NavigBar::sizeHint() const
{
    return QSize(NavigBarWidth,NavigBarHeight);
}