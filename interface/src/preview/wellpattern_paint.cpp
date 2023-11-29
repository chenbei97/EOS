/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\wellpattern_paint.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "wellpattern.h"

namespace V1 {
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
        //LOG<<mrows<<mcols<<mHoleInfo.count()<<mHoleInfo[0].count()<<mMousePos;
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
                    && !mDisablePoints[row][col]) { // 置灰不可选的点不要绘制鼠标高亮
                    path.clear();
                    path.moveTo(center);
                    path.addEllipse(center, radius * 0.75, radius * 0.75);
                    painter.fillPath(path, mMouseClickColor);
                }

                // (2) 绘制框选的所有孔
                if (mDrapPoints[row][col]
                    // && !mHoleInfo[row][col].isSelected //绘制拖拽临时的点,如果有已被选中的不需要再绘制,不过这样取消选中时不能绘制拖拽的点感受不好,还是恢复
                    && !mDisablePoints[row][col]) {  // 不可选的孔不允许绘制
                    path.clear(); //
                    path.moveTo(center);
                    path.addEllipse(center, radius * 0.75, radius * 0.75);
                    painter.fillPath(path, mMouseClickColor);
                }

                // (3) 绘制确实选中的孔和绘制孔内选中的点
                if (mHoleInfo[row][col].isselected && !mDisablePoints[row][col]) //  绘制确定选中的点,不可选的孔不允许绘制
                {
                    // (3.1) 绘制确实选中的孔
                    path.clear();
                    path.moveTo(center);
                    path.addEllipse(center,radius*0.75,radius*0.75);
                    painter.fillPath(path,mHoleInfo[row][col].color);

                    // (3.2) 绘制视野选中的区域对应的孔内小矩形区域
                    auto rects = mHoleInfo[row][col].viewrects; // 视野选中区域不为空
                    if (!rects.isEmpty()) {
                        // 1. 圆孔内圆的外接正方形的左上角顶点
                        auto topleft = center-QPointF(radius*0.75,radius*0.75);
                        for(auto rect: rects) { // 可对照ViewPattern::mapToSize的逻辑
                            auto x = rect.rect.topLeft().x() * radius * 1.5+ topleft.x();
                            auto y = rect.rect.topLeft().y() * radius * 1.5 + topleft.y();
                            auto w = rect.rect.size().width() * radius * 1.5;
                            auto h = rect.rect.size().height() * radius * 1.5;
                            if (rect.flag) { // rect.rect是0-1的,映射到圆孔需要基于当前的topleft,以及内圆的直径
                                painter.fillRect(QRectF(x,y,w,h),Qt::black);
                            } else {
                                painter.fillRect(QRectF(x,y,w,h),DefaultNativeColor);
                            }
                        }
                    }
                }

                // (4) 绘制不可选置灰的点,将其高亮颜色变为灰色,并画个x
                if (mDisablePoints[row][col]) {
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
        event->accept();
    }
}// end namespace v1