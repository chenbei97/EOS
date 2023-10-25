/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\photocanvas.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewphotocanvas.h"

PreviewPhotoCanvas::PreviewPhotoCanvas(QWidget *parent) : QWidget(parent)
{
    strategy = NoStrategy;
    mrows = 0;
    mcols = 0;
    mrectsize = DefaultPreviewPhotoCanvasViewRectSize;
}

void PreviewPhotoCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    switch (strategy) {
        case ExternalCircleRect: // 内接圆
            drawExternalCircle(painter);
            drawExternalCircleRect(painter);
            break;
        case InnerCircleRect:
            drawExternalCircle(painter);
            drawInnerLine(painter);
            break;
        default:
            break;
    }

    event->accept();
}

void PreviewPhotoCanvas::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}

void PreviewPhotoCanvas::drawInnerLine(QPainter &painter)
{
    auto radius = getExternalCircleRadius();
    auto p11 = QPoint(width()/2 - radius,0);
    auto p12 = QPoint(width()/2 + radius,0);
    auto p21 = QPoint(width()/2 - radius,height());
    auto p22 = QPoint(width()/2 + radius,height());

    painter.drawLine(p11,p21);
    painter.drawLine(p11,p12);
    painter.drawLine(p12,p22);
    painter.drawLine(p21,p22);
}

void PreviewPhotoCanvas::drawExternalCircle(QPainter &painter)
{
    auto radius = getExternalCircleRadius();
    painter.drawEllipse(QPoint(width()/2,height()/2),radius,radius);
}

int PreviewPhotoCanvas::getExternalCircleRadius() const
{ // 圆半径
    return width()>=height()?height()/2:width()/2;
}

int PreviewPhotoCanvas::getExternalCircleRectSize() const
{ // 圆内接正方形尺寸
    return getExternalCircleRadius()* sqrt(2);
}

QPoint PreviewPhotoCanvas::getExternalCircleRectTopLeftPoint() const
{ // 窗口内有个圆,圆的内部有个内接正方形,正方形的左上角顶点 (w/2-r*sqrt(2)/2,r-r*sqrt(2)/2)
    auto radius = getExternalCircleRadius();
    auto x = width()/2 - radius * sqrt(2)/2; // x坐标为窗口的一半减去半径的45°对边
    auto y = radius - radius * sqrt(2)/2; // y坐标为半径减去半径的45°对边

    return QPoint(x,y);
}

QPoint PreviewPhotoCanvas::getExternalCircleRectTopRightPoint() const
{ // 窗口内有个圆,圆的内部有个内接正方形,正方形的右上角顶点 (w/2+r*sqrt(2)/2,r-r*sqrt(2)/2)
    auto radius = getExternalCircleRadius();
    auto x = width()/2 + radius * sqrt(2)/2; // x坐标为窗口的一半加上半径的45°对边
    auto y = radius - radius * sqrt(2)/2; // y坐标和左上角相同

    return QPoint(x,y);
}

QPoint PreviewPhotoCanvas::getExternalCircleRectBottomLeftPoint() const
{// 窗口内有个圆,圆的内部有个内接正方形,正方形的左下角顶点 (w/2-r*sqrt(2)/2,r+r*sqrt(2)/2)
    auto radius = getExternalCircleRadius();
    auto x = width()/2 - radius * sqrt(2)/2; // x坐标和左上角相同
    auto y = radius + radius * sqrt(2)/2; // y坐标为半径加上半径的45°对边

    return QPoint(x,y);
}

QPoint PreviewPhotoCanvas::getExternalCircleRectBottomRightPoint() const
{// 窗口内有个圆,圆的内部有个内接正方形,正方形的右下角顶点 (w/2+r*sqrt(2)/2,r+r*sqrt(2)/2)
    auto radius = getExternalCircleRadius();
    auto x = width()/2 - radius * sqrt(2)/2; // x坐标和右上角相同
    auto y = radius + radius * sqrt(2)/2; // y坐标和左下角相同

    return QPoint(x,y);
}

void PreviewPhotoCanvas::drawExternalCircleRect(QPainter &painter) {
    auto rects = getExternalCircleRects();
    QPainterPath path;
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            path.clear();
            path.addRect(rects[r*mrows+c]);
            painter.fillPath(path,PreviewPhotoCanvasViewRectColor);
        }
    }

}

QRectVector PreviewPhotoCanvas::getExternalCircleRects() const
{ // 获取圆内的所有正方形区域
    QRectVector vec;
    auto rect_size = getExternalCircleRectSize(); // 内接正方形的尺寸,在这个尺寸内计算水平和垂直间距

    auto hor_gap = (rect_size-mrows * mrectsize) / (mrows+1);
    auto ver_gap = (rect_size-mcols * mrectsize) / (mcols+1);

    // 水平垂直偏移从正方形左上角顶点开始
    auto hor_offset = getExternalCircleRectTopLeftPoint().x();
    auto ver_offset = getExternalCircleRectTopLeftPoint().y();

    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            auto x = hor_gap + r * (mrectsize+hor_gap) + hor_offset;
            auto y = ver_gap + c * (mrectsize+ver_gap) + ver_offset;
            auto rect = QRect(x,y,mrectsize,mrectsize);
            vec.append(rect);
        }
    }
    return vec;
}

void PreviewPhotoCanvas::setStrategy(PreviewPhotoCanvas::DrawStrategy s)
{
    strategy = s;
    update();
}

void PreviewPhotoCanvas::setStrategy(PreviewPhotoCanvas::DrawStrategy s, int rows, int cols)
{ // 使用绘制视野圆策略使用的方法
    strategy = s;
    mrows = rows;
    mcols = cols;
    update();
}

void PreviewPhotoCanvas::setExternalCircleRectSize(int size)
{
    mrectsize = size;
}