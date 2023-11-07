/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\viewpattern_paint.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

void ViewPattern::drawSelectRect(QPainter &painter)
{ // 根据当前孔的组颜色去绘制视野已经被选择的小矩形区域

    // 计算当前孔的唯一id
    auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
    auto groupcolor = mCurrentViewInfo[HoleGroupColorField].toString();
    auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx

    if (mViewSelectPoints[idx].isEmpty()) return; // 删孔后清除缓存信息这里是空直接返回否则遍历越界

    auto  rects = getInnerRects();

    // 绘制框选的所有孔
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto rect = rects[row][col];
            if (mViewSelectPoints[idx][row][col]) {
                painter.fillRect(rect,groupcolor);
            }
//            else { // 不加也行,就是删除点时候要恢复成白色,默认也是白色
//                painter.fillRect(rect,Qt::white);
//            }
        }
    }
}

void ViewPattern::drawDrapRect(QPainter &painter)
{// 绘制推拽矩形框包含的小矩形区域
    auto  rects = getInnerRects();

    // 绘制框选的所有孔
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            if (mDrapPoints[row][col]) {
                auto rect = rects[row][col];
                painter.fillRect(rect,mMouseClickColor);
            }
        }
    }
    // 绘制框
    if (!mDrapRect.isNull()) {
        auto pen = painter.pen();
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawRect(mDrapRect);
        pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
        painter.setPen(pen);
    }
}

void ViewPattern::drawInnerLine(QPainter &painter)
{// 画窗口的线区分出小矩形区域
    // 绘制外边框
    auto p11 = getInnerRectTopLeftPoint();
    auto p12 = getInnerRectTopRightPoint();
    auto p21 = getInnerRectBottomLeftPoint();
    auto p22 = getInnerRectBottomRightPoint();

    painter.drawLine(p11,p21);
    painter.drawLine(p11,p12);
    painter.drawLine(p12,p22);
    painter.drawLine(p21,p22);

    // 绘制内部线
//    auto tops = getInnerVerticalLineTopPoints();
//    auto bottoms = getInnerVerticalLineBottomPoints();
//    auto lefts = getInnerHorizonalLineLeftPoints();
//    auto rights = getInnerHorizonalLineRightPoints();
//
//    Q_ASSERT(tops.count() == bottoms.count());
//    Q_ASSERT(lefts.count() == rights.count());
//    for(int i = 0; i < tops.count(); i++) {
//        painter.drawLine(tops.at(i),bottoms.at(i));
//    }
//    for(int i = 0; i < lefts.count(); i++) {
//        painter.drawLine(lefts.at(i),rights.at(i));
//    }
    // 优化一下改为直接画线,少做几次循环操作
    auto hor_offset = getInnerRectWidth();
    for (int i = 1; i <= mrows-1; ++i) {
        auto top = p11 + QPointF(i*hor_offset,0);
        auto bottom = p21 + QPointF(i*hor_offset,0);
        painter.drawLine(top,bottom);
    }
    auto ver_offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
    for (int i = 1; i <= mcols-1; ++i){
        auto top = p11 + QPointF(0,ver_offset*i);
        auto bottom = p12 + QPointF(0,ver_offset*i);
        painter.drawLine(top,bottom);
    }

    // 高亮鼠标区域
    auto rects = getInnerRects();
    //LOG<<"rects.size = "<<rects.size()<<" mouse = "<<mMousePoint;
    if (mMousePoint != QPoint(-1,-1)) // 尚未点击
        painter.fillRect(rects[mMousePoint.x()][mMousePoint.y()],mMouseClickColor);
}

void ViewPattern::drawCircle(QPainter &painter)
{// 画窗口的内接圆
    auto radius = getCircleRadius();
    painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
}


void ViewPattern::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    if (strategy == NoStrategy)
    {
        event->accept();
        return;
    }

    drawSelectRect(painter);
    drawCircle(painter);
    drawInnerLine(painter);
    drawDrapRect(painter);

    event->accept();
}
