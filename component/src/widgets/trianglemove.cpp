/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 09:29:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 09:29:53
 * @FilePath: \EOS\component\src\widgets\trianglemove.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "trianglemove.h"

TriangleMove::TriangleMove(QWidget *parent) : QWidget(parent)
{
    mLastPos = QPoint(-1,-1);
    trianglen = TriangleLength;
    trianggap = TriangleGap;

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    resize(TriangleMinSize,TriangleMinSize);
}

QSize TriangleMove::sizeHint() const
{
    return QSize(TriangleMinSize,TriangleMinSize);
}

void TriangleMove::setTriangleGap(double gap)
{
    trianggap = gap;
}

void TriangleMove::setTriangleLength(double len)
{
    trianglen = len;
}

void TriangleMove::mousePressEvent(QMouseEvent *event)
{
    mLastPos = event->pos();

    auto leftpoly = getLeftTrianglePoints();
    auto rightpoly = getRightTrianglePoints();
    auto toppoly = getTopTrianglePoints();
    auto bottompoly = getBottomTrianglePoints();

    if (leftpoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"left Triangle";
        emit leftTriangleClicked();
    } else if (rightpoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"right Triangle";
        emit rightTriangleClicked();
    } else if (toppoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"top Triangle";
        emit topTriangleClicked();
    } else if (bottompoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"botttom Triangle";
        emit bottomTriangleClicked();
    }

    event->accept();
}

void TriangleMove::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    painter.drawPolygon(getLeftTrianglePoints());
    painter.drawPolygon(getRightTrianglePoints());
    painter.drawPolygon(getTopTrianglePoints());
    painter.drawPolygon(getBottomTrianglePoints());
}


QPolygonF TriangleMove::getLeftTrianglePoints() const
{
    auto vertex = QPointF(0,height()/2);
    auto p_top = QPointF(trianglen*sqrt(3)/2,height()/2-trianglen/2);
    auto p_bottom = QPointF(trianglen*sqrt(3)/2,height()/2+trianglen/2);

    vertex += QPointF(trianggap,0);
    p_top += QPointF(trianggap,0);
    p_bottom += QPointF(trianggap,0);

    return QPolygonF(QPointFVector()<<vertex<<p_top<<p_bottom);
}

QPolygonF TriangleMove::getRightTrianglePoints() const
{
    auto vertex = QPointF(width(),height()/2);
    auto p_top = QPointF(width()-trianglen*sqrt(3)/2,height()/2-trianglen/2);
    auto p_bottom = QPointF(width()-trianglen*sqrt(3)/2,height()/2+trianglen/2);

    vertex += QPointF(-trianggap,0);
    p_top += QPointF(-trianggap,0);
    p_bottom += QPointF(-trianggap,0);

    return QPolygonF(QPointFVector()<<vertex<<p_top<<p_bottom);
}

QPolygonF TriangleMove::getTopTrianglePoints() const
{
    auto vertex = QPointF(width()/2,0);
    auto p_left = QPointF(width()/2-trianglen/2,trianglen*sqrt(3)/2);
    auto p_right = QPointF(width()/2+trianglen/2,trianglen*sqrt(3)/2);

    vertex += QPointF(0,trianggap);
    p_left += QPointF(0,trianggap);
    p_right += QPointF(0,trianggap);

    return QPolygonF(QPointFVector()<<vertex<<p_left<<p_right);
}

QPolygonF TriangleMove::getBottomTrianglePoints() const
{
    auto vertex = QPointF(width()/2,height());
    auto p_left = QPointF(width()/2-trianglen/2,height()-trianglen*sqrt(3)/2);
    auto p_right = QPointF(width()/2+trianglen/2,height()-trianglen*sqrt(3)/2);

    vertex += QPointF(0,-trianggap);
    p_left += QPointF(0,-trianggap);
    p_right += QPointF(0,-trianggap);

    return QPolygonF(QPointFVector()<<vertex<<p_left<<p_right);
}