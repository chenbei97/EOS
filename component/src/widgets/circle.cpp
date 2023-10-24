/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 16:55:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 17:14:55
 * @FilePath: \EOS\component\src\widgets\circle.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "circle.h"

Circle::Circle(QWidget *parent) : QWidget(parent)
{

}

void Circle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    painter.drawEllipse(getCenter(),getRadius(),getRadius());

    event->accept();
}

void Circle::mousePressEvent(QMouseEvent *event)
{
    mMousePos = QPoint(-1,-1);
    auto pos = event->pos();
    QPainterPath path;
    path.addEllipse(getCenter(),getRadius(),getRadius());
    if (path.contains(pos)){

        auto pos_x = pos.x();
        auto pos_y = pos.y();

        if (pos_x>=width()/2 && pos_y<=height()/2) {
            // (400,0)应该映射为(200,200) 其中radius=200
            mMousePos.setX(pos_x-getRadius());
            mMousePos.setY(-(pos_y-getRadius()));
        }
        LOG<<"pos = "<<mMousePos;
    }
    emit mouseClicked(mMousePos);
    event->accept();
}

QPoint Circle::getCenter() const
{
    return QPoint(width()/2,height()/2);
}

int Circle::getRadius() const
{
    return (width()>=height()?height():width())/2;
}