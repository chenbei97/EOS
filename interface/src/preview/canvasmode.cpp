/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 13:59:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 14:00:04
 * @FilePath: \EOS\interface\src\preview\canvasmode.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "canvasmode.h"

CanvasMode::CanvasMode(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum); // 代替setFixedSize
}

void CanvasMode::changeMode(CanvasMode::canvasMode mode)
{
    canvasmode = mode;
    emit cameraModeChanged(canvasmode);
    update();
}

void CanvasMode::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(1);
    painter.setPen(pen);

    // 准备变量【】【|】【】 竖线是width/2,故第1个的x坐标是w/2-mw*1.5,第2个是w/2-mw*0.5,第3个是w/2+mw*0.5,这里mw是矩形的宽度
    auto mw = PhotoFieldMetricWidth*2.0; // 宽度2倍是不想绘制的矩形看起来太小
    auto x_1 = width()/2 - mw*1.5; // 以文字长的为准,photo是5个字母,video也是,live是4个字母
    auto x_2 = width()/2 - mw*0.5;
    auto x_3 = width()/2 + mw*0.5;

    // 绘制高亮
    QPainterPath path;
    switch (canvasmode) {
        case LiveMode:
            path.addRect(x_1,0,mw,height());
            break;
        case PhotoMode:
            path.addRect(x_2,0,mw,height());
            break;
        case VideoMode:
            path.addRect(x_3,0,mw,height());
            break;
    }
    painter.fillPath(path,QColor("#B5E61D"));

    // 绘制文字
    double factor = 7/5.0; // 高度微调因子
    path.clear(); // live字段会比video和photo小,让文字居中需要(mw-w)/2
    path.addText(x_1 + (mw-LiveFieldMetricWidth)/2,(height()-DefaultPainterPixelHeight)*factor,DefaultPainterFont,LiveField);
    painter.fillPath(path,Qt::black);
    path.clear(); // video和photo文字就在矩形左边框就可以
    path.addText(x_2 + PhotoFieldMetricWidth/2,(height()-DefaultPainterPixelHeight)*factor,DefaultPainterFont,PhotoField);
    painter.fillPath(path,Qt::black);
    path.clear();
    path.addText(x_3 + PhotoFieldMetricWidth/2,(height()-DefaultPainterPixelHeight)*factor,DefaultPainterFont,VideoField);
    painter.fillPath(path,Qt::black);

    // 绘制框线,最后防止被高亮覆盖
    painter.drawRect(x_1,0,mw,height());
    painter.drawRect(x_2,0,mw,height());
    painter.drawRect(x_3,0,mw,height());
    event->accept();
}

void CanvasMode::mousePressEvent(QMouseEvent *event)
{
    auto mw = PhotoFieldMetricWidth*2.0; // 宽度2倍是不想绘制的矩形看起来太小
    auto x_1 = width()/2 - mw*1.5; // 以文字长的为准,photo是5个字母,video也是,live是4个字母
    auto x_2 = width()/2 - mw*0.5;
    auto x_3 = width()/2 + mw*0.5;
    QRect r1(x_1,0,mw,height());
    QRect r2(x_2,0,mw,height());
    QRect r3(x_3,0,mw,height());

    if (r1.contains(event->pos())) canvasmode = LiveMode;
    else if (r2.contains(event->pos())) canvasmode = PhotoMode;
    else if (r3.contains(event->pos())) canvasmode = VideoMode;

    emit cameraModeChanged(canvasmode); // live是0,photo是1,video是2
    event->accept();
    update();
}

QSize CanvasMode::sizeHint() const
{
    return QSize(CameraModeHeight,CameraModeHeight);
}

QSize CanvasMode::minimumSizeHint() const
{
    return QSize(CameraModeHeight,CameraModeHeight);
}