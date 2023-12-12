/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\wellpattern_paint.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "wellpattern.h"

QRectF2DVector WellPattern::getHoleRectsOnViewSize(const QPoint &coordinate) const
{// 拿到某个孔基于视野窗口尺寸来划分的所有小矩形区域

    // 1. 获取孔内圆的半径+圆心坐标
    auto cell_size = getChildSize();
    double cell_w = cell_size.width();
    double cell_h = cell_size.height();
    double radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内
    radius = radius * 0.75;
    auto  centerPts = getCenterPoints();
    auto center  = centerPts[coordinate.x()][coordinate.y()]; // 圆心

    // 2. 根据圆心和内圆半径拿到内圆外接正方形区域
    auto rect = QRectF(center,QSize(radius*2,radius*2)); // 这个孔内圆的外接正方形

    // 3. 拿到这个孔存储的视野尺寸信息
    auto viewrows = mHoleInfo[coordinate.x()][coordinate.y()].viewsize;
    auto viewcols = viewrows;

    // 4. 得到外接正方形根据视野尺寸划分后每个小矩形的长度和宽度
    QRectF2DVector m;
    auto hoffset = 2 * radius / viewrows *1.0; // 整个外接正方形的尺寸就是2*radius
    auto voffset = 2 * radius / viewcols *1.0;

    // 4.拿到外界正方形的左上角顶点
    auto start = center-QPointF(radius,radius); // 圆心坐标减去水平垂直距离radius即可

    // 5. 有了左上角起点+每次水平垂直移动的小矩形尺寸就可以得到所有小矩形的区域
    for(int i = 0 ; i < viewrows; ++i) {
        QRectFVector vec;
        for(int j = 0; j < viewcols; ++j) { // j*offset加在x坐标也就是水平方向
            auto topleft = start + QPointF(j*hoffset,i*voffset); // x, x+d，j依次取0,1; y先不变在后
            auto bottomright = topleft + QPointF(hoffset,voffset);
            vec.append(QRectF(topleft,bottomright));
        }
        m.append(vec);
    }
    return m;
}

QRectF2DVector WellPattern::getHoleRectsOnViewSize(int x,int y) const
{
    return getHoleRectsOnViewSize(QPoint(x,y));
}

void WellPattern::paintEvent(QPaintEvent *event)
{
    Pattern::paintEvent(event);

    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();
    int radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内
    //auto  centerPts = getCenterPoints();// 改为直接计算,减少循环

    QPainter painter(this);
    QPainterPath path;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            //auto center = centerPts[row][col]; // 改为直接计算,减少循环
            auto center  = QPointF(mGap+cell_w/2+mSpace+(cell_w+mSpace)*col,
                                   mGap+cell_h/2+mSpace+(cell_h+mSpace)*row);
            // (1) 绘制鼠标点击的高亮
            //启用了鼠标事件mMousePos才会被赋值,否则是(-1,-1),所以这里不用再判断是否启用了鼠标事件
            if (mMousePos.x() == row && mMousePos.y() == col
                && !mHoleInfo[row][col].isselected// 已绘制的点不要绘制鼠标选中高亮
                && !mDisableHoles[row][col]) { // 置灰不可选的点不要绘制鼠标高亮
                path.clear();
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, mMouseClickColor);
            }

            // (2) 绘制框选的所有孔
            if (mDrapHoles[row][col]
                // && !mHoleInfo[row][col].isSelected //绘制拖拽临时的点,如果有已被选中的不需要再绘制,不过这样取消选中时不能绘制拖拽的点感受不好,还是恢复
                && !mDisableHoles[row][col]) {  // 不可选的孔不允许绘制
                path.clear(); //
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, mMouseClickColor);
            }

            // (3) 绘制确实选中的孔和绘制孔内选中的点
            if (mHoleInfo[row][col].isselected && !mDisableHoles[row][col]) //  绘制确定选中的点,不可选的孔不允许绘制
            {
                // (3.1) 绘制确实选中的孔
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,mHoleInfo[row][col].color);

                // 对于区域模式有3种方法绘制
                if (mSelectMode == RectMode) {
                    // (3.2) 第1种绘制法: 直接绘制视野区域
//                    auto rects = mHoleInfo[row][col].viewrects; // 视野选中区域不为空
//                    if (!rects.isEmpty()) {
//                        auto topleft = center-QPointF(radius*0.75,radius*0.75);// 圆孔内圆的外接正方形的左上角顶点
//                        for(auto rect: rects) { // 可对照ViewPattern::mapToSize的逻辑
//                            auto x = rect.topLeft().x() * radius * 1.5+ topleft.x();
//                            auto y = rect.topLeft().y() * radius * 1.5 + topleft.y();
//                            auto w = rect.size().width() * radius * 1.5;
//                            auto h = rect.size().height() * radius * 1.5;
//                            painter.fillRect(QRectF(x,y,w,h),Qt::black);
//                        }
//                    }
                    // (3.3) 第2种绘制法: 只是绘制点,相比于第3种比较有效率,以点带面(但是只有点,没有面不采用)
//                    auto points = mHoleInfo[row][col].uipoints;
//                    if (!points.isEmpty()) {
//                        auto pen = painter.pen();
//                        pen.setWidth(4);
//                        painter.setPen(pen);
//                        auto topleft = center-QPointF(radius*0.75,radius*0.75);// 圆孔内圆的外接正方形的左上角顶点
//                        for(auto point: points) {
//                            auto x = point.x() * radius * 1.5 + topleft.x();
//                            auto y = point.y() * radius * 1.5 + topleft.y();
//                            painter.drawPoint(QPointF(x,y));
//                        }
//                        pen = painter.pen();
//                        pen.setWidth(2);
//                        painter.setPen(pen);
//                    }
                    // (3.4) 第3种绘制法: 把视野点整体缩放,单位小矩形的尺寸取决于viewPattern是怎么定义缩放比例的(没有效率不采用)
                    auto points = mHoleInfo[row][col].uipoints;
                    if (!points.isEmpty()) {
                        auto topleft = center-QPointF(radius*0.75,radius*0.75);// 圆孔内圆的外接正方形的左上角顶点
                        if ((points.count() > mDispersedMaskSize*mDispersedMaskSize*0.2 && mrows == 16) // 384 视野上孔很小没必要做循环了,数量太多直接画个黑圆就得
                            || (points.count() > mDispersedMaskSize*mDispersedMaskSize*0.9 && mrows == 8)) { // 96 这个可以超过9000太多时忽略掉,其他时候不卡正常绘制
                            painter.fillRect(QRectF(topleft,QSizeF(radius*1.5,radius*1.5)),Qt::black);
                        } else { // 对于384和96的一些优化,因为计算量比较大,而且wellpattern的孔很小时就没有必要按点画了,点特别多时看不出来直接画区域
                            for(auto point: points) {
                                auto x = point.x() * radius * 1.5 + topleft.x();
                                auto y = point.y() * radius * 1.5 + topleft.y();
                                auto w = 1.0 / mDispersedMaskSize * radius * 1.5; // viewPattern传来的坐标就是根据mViewMaskSize去缩放的
                                auto h = 1.0 / mDispersedMaskSize * radius * 1.5; // 那么w,h也是根据这个倍数去缩放
                                painter.fillRect(QRectF(x,y,w,h),Qt::black);
                            }
                        }
                    } // end 3.4
                }
                else if(mSelectMode == PointMode) {// end rectmode ,如果是点模式可以通过电机坐标来绘制,电机坐标就是视野离散坐标
                    auto points = mHoleInfo[row][col].viewpoints;
                    if (!points.isEmpty()) {
                        auto pen = painter.pen();
                        pen.setWidth(DefaultDrawPointWidth);
                        painter.setPen(pen);
                        auto topleft = center-QPointF(radius*0.75,radius*0.75);// 圆孔内圆的外接正方形的左上角顶点
                        for(auto point: points) {
                            auto x = point.x() * radius * 1.5 + topleft.x();
                            auto y = point.y() * radius * 1.5 + topleft.y();
                            painter.drawPoint(x,y);
                        }
                        pen = painter.pen();
                        pen.setWidth(DefaultPainterPenWidth);
                        painter.setPen(pen);
                    }
                }
            }

            // (4) 绘制不可选置灰的点,将其高亮颜色变为灰色,并画个x
            if (mDisableHoles[row][col]) {
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius,radius);
                painter.fillPath(path,mInnerCircleColor);

                auto p11 = center + QPointF(radius*sqrt(2)/2,-radius* sqrt(2)/2);
                auto p12 = center - QPointF(radius*sqrt(2)/2,-radius* sqrt(2)/2);
                auto p21 = center + QPointF(radius*sqrt(2)/2,radius* sqrt(2)/2);
                auto p22 = center - QPointF(radius*sqrt(2)/2,radius* sqrt(2)/2);
                painter.drawLine(p11,p12);
                painter.drawLine(p21,p22);
            }
        }
    }
    // (5) 绘制框选框(放最后画,防止被填充颜色覆盖)
    if (!mDrapRect.isNull()) {
        auto pen = painter.pen();
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawRect(mDrapRect);
        pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
        painter.setPen(pen);
    }
}
