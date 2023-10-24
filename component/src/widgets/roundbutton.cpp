/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 09:01:34
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 09:21:47
 * @FilePath: \EOS\component\src\widgets\roundbutton.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "roundbutton.h"

RoundButton::RoundButton(QWidget *parent): PushButton(parent)
{
    strategy = NoStrategy;
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    resize(RoundButtonSize,RoundButtonSize);
}

void RoundButton::setSize(int size)
{
    setFixedSize(size,size);
}

void RoundButton::setStrategy(RoundButton::PaintStrategy s)
{
    strategy = s;
    update();
}

void RoundButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    switch (strategy) {
        case RightTriangle:
            drawRightTriangle(painter);
            break;
        case LeftTriangle:
            drawLeftTriangle(painter);
            break;
        case TopTriangle:
            drawTopTriangle(painter);
            break;
        case BottomTriangle:
            drawBottomTriangle(painter);
            break;
        default:
            break;
    }
    //PushButton::paintEvent(event);
    painter.drawRect(0,0,width(),height());
    event->accept();
}

void RoundButton::drawRightTriangle(QPainter &painter)
{ // 4等分,右边3份用于绘制三角形,三条边
    const int num = 4;
    auto p1 = QPoint(width()/num,0);
    auto p2 = QPoint(width()/num,height());
    auto p3 = QPoint(width(),height()/2);

    painter.drawLine(p1,p2);
    painter.drawLine(p2,p3);
    painter.drawLine(p3,p1);
}

void RoundButton::drawLeftTriangle(QPainter &painter)
{
    const int num = 4;
    auto p1 = QPoint(width()/num*(num-1),0);
    auto p2 = QPoint(width()/num*(num-1),height());
    auto p3 = QPoint(0,height()/2);

    painter.drawLine(p1,p2);
    painter.drawLine(p2,p3);
    painter.drawLine(p3,p1);
}

void RoundButton::drawTopTriangle(QPainter &painter)
{
    const int num = 4;
    auto p1 = QPoint(0,height()/num*(num-1));
    auto p2 = QPoint(width(),height()/num*(num-1));
    auto p3 = QPoint(width()/2,0);

    painter.drawLine(p1,p2);
    painter.drawLine(p2,p3);
    painter.drawLine(p3,p1);
}

void RoundButton::drawBottomTriangle(QPainter &painter)
{
    const int num = 4;
    auto p1 = QPoint(0,height()/num);
    auto p2 = QPoint(width(),height()/num);
    auto p3 = QPoint(width()/2,height());

    painter.drawLine(p1,p2);
    painter.drawLine(p2,p3);
    painter.drawLine(p3,p1);
}

QSize RoundButton::sizeHint() const
{
    return QSize(RoundButtonSize,RoundButtonSize);
}