/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\viewpattern_geometry.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

double ViewPattern::getCircleRadius() const
{ // 视野圆半径
    return width()>=height()?height()/2.0:width()/2.0;
}

double ViewPattern::getInnerRectWidth() const
{// 小矩形区域的宽度
    return 2 * getCircleRadius() / mrows *1.0;
}

double ViewPattern::getInnerRectHeight() const
{// 小矩形区域的高度
    return 2 * getCircleRadius() / mcols *1.0;
}

QPointF ViewPattern::getInnerRectTopLeftPoint() const
{// 外接正方形左上角顶点
    return QPointF(width()/2.0-getCircleRadius(),0);
}

QPointF ViewPattern::getInnerRectTopRightPoint() const
{// 外接正方形右上角顶点
    return QPointF(width()/2.0+getCircleRadius(),0);
}

QPointF ViewPattern::getInnerRectBottomLeftPoint() const
{// 外接正方形左下角顶点
    return QPointF(width()/2.0-getCircleRadius(),height());
}

QPointF ViewPattern::getInnerRectBottomRightPoint() const
{// 外接正方形右下角顶点
    return QPointF(width()/2.0+getCircleRadius(),height());
}

QPointFVector ViewPattern::getInnerVerticalLineTopPoints() const
{// 正方形顶侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectWidth();
    auto topleft= getInnerRectTopLeftPoint();
    for (int i = 1; i <= mrows-1; ++i)
        vec.append(topleft+ QPoint(i*offset,0));
//    LOG<<"topleft = "<<topleft<<" offset = "<<offset<<" mrows = "<<mrows
//    <<" radius = "<<getExternalCircleRadius();
    return vec;
}

QPointFVector ViewPattern::getInnerVerticalLineBottomPoints() const
{// 正方形底侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectWidth();
    auto bottomleft= getInnerRectBottomLeftPoint();
    for (int i = 1; i <= mrows-1; ++i)
        vec.append(bottomleft+ QPoint(i*offset,0));
    return vec;
}

QPointFVector ViewPattern::getInnerHorizonalLineLeftPoints() const
{// 正方形左侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
    auto topleft = getInnerRectTopLeftPoint();
    for (int i = 1; i <= mcols-1; ++i) // i = 1开始是不包含topleft
        vec.append(topleft+ QPoint(0,i*offset)); // x不变,y增加
    return vec;
}

QPointFVector ViewPattern::getInnerHorizonalLineRightPoints() const
{// 正方形右侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
    auto topright = getInnerRectTopRightPoint();
    for (int i = 1; i <= mcols-1; ++i) // i = 1开始是不包含topright
        vec.append(topright+ QPoint(0,i*offset)); // x不变,y增加
    return vec;
}

QRectF2DVector ViewPattern::getInnerRects() const
{
    QRectF2DVector m;
    auto hoffset = getInnerRectWidth();
    auto voffset = getInnerRectHeight();

    auto start = getInnerRectTopLeftPoint();
    for(int i = 0 ; i < mrows; ++i) {
        QRectFVector vec;
        for(int j = 0; j < mcols; ++j) { // j*offset加在x坐标也就是水平方向
            auto topleft = start + QPointF(j*hoffset,i*voffset); // x, x+d，j依次取0,1; y先不变在后
            auto bottomright = topleft + QPointF(hoffset,voffset);
            vec.append(QRectF(topleft,bottomright));
        }
        m.append(vec);
    }

    return m;
}

