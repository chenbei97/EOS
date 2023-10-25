/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 13:59:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 14:00:04
 * @FilePath: \EOS\interface\src\preview\cameramode.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "cameramode.h"

CameraMode::CameraMode(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum); // 代替setFixedSize
}

void CameraMode::changeMode(CameraMode::cameraMode mode)
{
    mode == LiveMode? isLive=true: isLive= false;
    update();
}

void CameraMode::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(1);
    painter.setPen(pen);

    // 绘制文字和矩形框
    auto x_1 = width()/2-PhotoFieldMetricWidth;
    auto x_2 = width()/2+PhotoFieldMetricWidth;
    painter.drawRect(x_1,0,PhotoFieldMetricWidth*2,height());
    painter.drawRect(x_2,0,PhotoFieldMetricWidth*2,height());

    QPainterPath path;
    // 绘制高亮
    if (isLive) {
        path.addRect(x_1,0,PhotoFieldMetricWidth*2,height());
    } else {
        path.addRect(x_2,0,PhotoFieldMetricWidth*2,height());
    }
    painter.fillPath(path,QColor("#B5E61D"));

    path.clear();
    path.addText(x_1 + (2*PhotoFieldMetricWidth-LiveFieldMetricWidth)/2,(height()-DefaultPainterPixelHeight)*4/3,DefaultPainterFont,LiveField);
    painter.fillPath(path,Qt::black);
    path.clear();
    path.addText(x_2 + PhotoFieldMetricWidth/2,(height()-DefaultPainterPixelHeight)*4/3,DefaultPainterFont,PhotoField);
    painter.fillPath(path,Qt::black);
    event->accept();
}

void CameraMode::mousePressEvent(QMouseEvent *event)
{
    auto x_1 = width()/2-PhotoFieldMetricWidth;
    auto x_2 = width()/2+PhotoFieldMetricWidth;
    QRect r1(x_1,0,PhotoFieldMetricWidth*2,height());
    QRect r2(x_2,0,PhotoFieldMetricWidth*2,height());
    if (r1.contains(event->pos())) isLive = true;
    else if (r2.contains(event->pos())) isLive = false;

    emit cameraModeChanged(!isLive); // live是0,photo是1
    event->accept();
    update();
}

QSize CameraMode::sizeHint() const
{
    return QSize(CameraModeHeight,CameraModeHeight);
}
