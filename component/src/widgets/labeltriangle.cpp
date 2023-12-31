/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 11:36:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-14 11:49:01
 * @FilePath: \EOS\component\src\widgets\labeltriangle.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "labeltriangle.h"

LabelTriangle::LabelTriangle(QWidget*parent):Label(parent)
{
    mLastPos = QPoint(-1,-1);
    highcolor  = Qt::green;
    highcolor.setAlpha(DefaultColorAlpha);
    trianglen = PhotoCanvasTriangleLength;
    trianggap = PhotoCanvasTriangleGap;
}

void LabelTriangle::mouseReleaseEvent(QMouseEvent *event)
{
    mLastPos = QPoint(-1,-1);
    update();
}

void LabelTriangle::mousePressEvent(QMouseEvent *event)
{
    mLastPos = event->pos();
    update();

    auto leftpoly = getLeftTrianglePoints();
    auto rightpoly = getRightTrianglePoints();
    auto toppoly = getTopTrianglePoints();
    auto bottompoly = getBottomTrianglePoints();

    if (leftpoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"left Triangle";
        emit leftTriangleClicked();
        emit triangleClicked(0);
    } else if (rightpoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"right Triangle";
        emit rightTriangleClicked();
        emit triangleClicked(2);
    } else if (toppoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"top Triangle";
        emit topTriangleClicked();
        emit triangleClicked(1);
    } else if (bottompoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"botttom Triangle";
        emit bottomTriangleClicked();
        emit triangleClicked(3);
    }

    event->accept();
}

void LabelTriangle::paintEvent(QPaintEvent *event)
{
    Label::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    auto left = getLeftTrianglePoints();
    auto right = getRightTrianglePoints();
    auto top = getTopTrianglePoints();
    auto bottom = getBottomTrianglePoints();

    painter.drawPolygon(left);
    painter.drawPolygon(right);
    painter.drawPolygon(top);
    painter.drawPolygon(bottom);

    if (mLastPos == QPoint(-1,-1)) return;

    QPainterPath path;
    if (left.containsPoint(mLastPos,Qt::WindingFill))
        path.addPolygon(left);
    else if (right.containsPoint(mLastPos,Qt::WindingFill))
        path.addPolygon(right);
    else if (top.containsPoint(mLastPos,Qt::WindingFill))
        path.addPolygon(top);
    else if (bottom.containsPoint(mLastPos,Qt::WindingFill))
        path.addPolygon(bottom);

    painter.fillPath(path,highcolor);
}


QPolygonF LabelTriangle::getLeftTrianglePoints() const
{
    auto vertex = QPointF(0,height()/2);
    auto p_top = QPointF(trianglen*sqrt(3)/2,height()/2-trianglen/2);
    auto p_bottom = QPointF(trianglen*sqrt(3)/2,height()/2+trianglen/2);

    vertex += QPointF(trianggap,0);
    p_top += QPointF(trianggap,0);
    p_bottom += QPointF(trianggap,0);

    return QPolygonF(QPointFVector()<<vertex<<p_top<<p_bottom);
}

QPolygonF LabelTriangle::getRightTrianglePoints() const
{
    auto vertex = QPointF(width(),height()/2);
    auto p_top = QPointF(width()-trianglen*sqrt(3)/2,height()/2-trianglen/2);
    auto p_bottom = QPointF(width()-trianglen*sqrt(3)/2,height()/2+trianglen/2);

    vertex += QPointF(-trianggap,0);
    p_top += QPointF(-trianggap,0);
    p_bottom += QPointF(-trianggap,0);

    return QPolygonF(QPointFVector()<<vertex<<p_top<<p_bottom);
}

QPolygonF LabelTriangle::getTopTrianglePoints() const
{
    auto vertex = QPointF(width()/2,0);
    auto p_left = QPointF(width()/2-trianglen/2,trianglen*sqrt(3)/2);
    auto p_right = QPointF(width()/2+trianglen/2,trianglen*sqrt(3)/2);

    vertex += QPointF(0,trianggap);
    p_left += QPointF(0,trianggap);
    p_right += QPointF(0,trianggap);

    return QPolygonF(QPointFVector()<<vertex<<p_left<<p_right);
}

QPolygonF LabelTriangle::getBottomTrianglePoints() const
{
    auto vertex = QPointF(width()/2,height());
    auto p_left = QPointF(width()/2-trianglen/2,height()-trianglen*sqrt(3)/2);
    auto p_right = QPointF(width()/2+trianglen/2,height()-trianglen*sqrt(3)/2);

    vertex += QPointF(0,-trianggap);
    p_left += QPointF(0,-trianggap);
    p_right += QPointF(0,-trianggap);

    return QPolygonF(QPointFVector()<<vertex<<p_left<<p_right);
}