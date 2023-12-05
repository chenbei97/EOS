/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\viewpattern_paint.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

void ViewPattern::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(DefaultPainterPenWidth);
    painter.setPen(pen);
    painter.drawRect(rect());

    // 1. 变量准备
    auto radius = width()>=height()?height()/2.0:width()/2.0;
    auto diameter = 2.0 * radius;
    auto p11 = getInnerRectTopLeftPoint();
    auto p12 = getInnerRectTopRightPoint();
    auto p21 = getInnerRectBottomLeftPoint();
    auto p22 = getInnerRectBottomRightPoint();
    auto groupcolor = mViewInfo[HoleGroupColorField].toString();
    auto id = holeID();

    if (mSelectMode == RectMode) { // 区域模式
        // 2. 绘制选中的区域
        // 以下2种绘制法都可以,和wellpattern绘制对应,wellpattern也是2种绘制法
        // 1. 直接绘制视野区域 对应wellpattern的(3.2)画法
//            if (!mViewRects[id].isEmpty()) {
//                for(auto viewRect: mViewRects[id]) {
//                    if (viewRect.flag)
//                        painter.fillRect(mapToSize(viewRect.rect,p11,diameter,diameter),groupcolor);
//                    else painter.fillRect(mapToSize(viewRect.rect,p11,diameter,diameter),DefaultNativeColor);
//                }
//            }

        // 2. 使用掩码矩阵点的缩放区域绘制,和wellpattern的mViewMaskSize一致就可 对应(3.4)画法
        //LOG<<mUiViewMaskNormPoints[holeID()].count()<<diameter;
        //LOG<<"id = "<<id<<mUiViewMaskNormPoints[id].count();
        for(auto pt: mUiViewMaskNormPoints[id]) {
                auto w = 1.0 / mUiViewMaskSize;
                auto h = 1.0 / mUiViewMaskSize;
                auto viewRect = QRectF(pt.x.toDouble(),pt.y.toDouble(),w,h);
                //LOG<<mapToSize(viewRect,p11,diameter,diameter);
                painter.fillRect(mapToSize(viewRect,p11,diameter,diameter),groupcolor);
        }

        // 3. 绘制圆,外接正方形和便于区分的灰色网格线

        painter.drawLine(p11,p21);
        painter.drawLine(p12,p22);
        painter.drawLine(p11,p12);
        painter.drawLine(p21,p22);
        pen = painter.pen();
        pen.setWidth(1);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        auto hor_offset = getInnerRectWidth();
        for (int i = 1; i <= mSize-1; ++i) {
            auto top = p11 + QPointF(i*hor_offset,0);
            auto bottom = p21 + QPointF(i*hor_offset,0);
            painter.drawLine(top,bottom);
        }
        auto ver_offset = getInnerRectHeight();
        for (int i = 1; i <= mSize-1; ++i){
            auto top = p11 + QPointF(0,ver_offset*i);
            auto bottom = p12 + QPointF(0,ver_offset*i);
            painter.drawLine(top,bottom);
        }

        pen = painter.pen();
        pen.setWidth(DefaultPainterPenWidth);
        pen.setColor(Qt::blue);
        painter.setPen(pen);

        // 4. 鼠标单击生成的矩形框
        painter.drawRect(mapToSize(mMouseRect,p11,diameter,diameter));
        // 5. 鼠标拖拽生成的矩形框
        if (!mDrapRectF.isEmpty()) {
            painter.drawRect(mapToSize(mDrapRectF,p11,diameter,diameter));
        }

        pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
        painter.setPen(pen);
    } else { // 点模式
        pen = painter.pen();
        pen.setWidth(DefaultDrawPointWidth);
        painter.setPen(pen);
        //LOG<<mUiSelectedPoints[id];
        for(auto pt:mUiSelectedPoints[id]) {
            painter.drawPoint(mapToPointF(pt));
        }
        painter.drawPoint(mMousePos);
        pen = painter.pen();
        pen.setWidth(DefaultPainterPenWidth);
        painter.setPen(pen);
    }
    // 画圆
    painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);

    event->accept();
}


QRectF ViewPattern::getValidRect() const
{ //有效的区域是整个圆和外接正方形,点在外边不认为有效,防止用户把viewwindow窗口拉伸后点
    return QRectF(getInnerRectTopLeftPoint(),QSize(getCircleRadius()*2.0,getCircleRadius()*2.0));
}

double ViewPattern::getCircleRadius() const
{ // 视野圆半径
    return width()>=height()?height()/2.0:width()/2.0;
}

double ViewPattern::getInnerRectWidth() const
{// 小矩形区域的宽度
    return 2.0 * getCircleRadius() /  mSize *1.0;
}

double ViewPattern::getInnerRectHeight() const
{// 小矩形区域的高度
    return 2.0 * getCircleRadius() / mSize *1.0;
}

QPointF ViewPattern::getInnerRectTopLeftPoint() const
{// 外接正方形左上角顶点
    if (width()>=height())
        return QPointF(width()/2.0-getCircleRadius(),0.0);
    else
        return QPointF(0.0,height()/2.0-getCircleRadius()); // 左上角x=0
}

QPointF ViewPattern::getInnerRectTopRightPoint() const
{// 外接正方形右上角顶点
    if (width()>=height())
        return QPointF(width()/2.0+getCircleRadius(),0.0);
    else
        return QPointF(width(),height()/2.0-getCircleRadius());
}

QPointF ViewPattern::getInnerRectBottomLeftPoint() const
{// 外接正方形左下角顶点
    if (width()>=height())
        return QPointF(width()/2.0-getCircleRadius(),height());
    else
        return QPointF(0,height()/2.0+getCircleRadius());
}

QPointF ViewPattern::getInnerRectBottomRightPoint() const
{// 外接正方形右下角顶点
    if (width()>=height())
        return QPointF(width()/2.0+getCircleRadius(),height());
    else
        return QPointF(width(),height()/2.0+getCircleRadius());
}

