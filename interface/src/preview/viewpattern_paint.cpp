/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\viewpattern_paint.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

inline namespace V2 {
    void ViewPattern::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        auto pen = painter.pen();
        pen.setWidth(2);
        painter.setPen(pen);

        auto radius = width()>=height()?height()/2.0:width()/2.0;
        auto diameter = 2.0 * radius;
        auto p11 = getInnerRectTopLeftPoint();
        auto p12 = getInnerRectTopRightPoint();
        auto p21 = getInnerRectBottomLeftPoint();
        auto p22 = getInnerRectBottomRightPoint();
        //painter.fillRect(rect(),Qt::white);

        auto groupcolor = mViewInfo[HoleGroupColorField].toString();

        // 以下2种绘制法都可以,和wellpattern绘制对应

//        if (!mViewRects[holeID()].isEmpty()) {
//            for(auto viewRect: mViewRects[holeID()]) {
//                if (viewRect.flag)
//                    painter.fillRect(mapToSize(viewRect.rect,p11,diameter,diameter),groupcolor);
//                else painter.fillRect(mapToSize(viewRect.rect,p11,diameter,diameter),DefaultNativeColor);
//            }
//        }

        //LOG<<mUiViewMaskNormPoints[holeID()].count()<<diameter;
        // 新的写法,,wellpattern对应也使用点来绘制区域,2者的mViewMaskSize一致就可
        auto id = holeID();
        LOG<<"id = "<<id<<mUiViewMaskNormPoints[id].count();
        for(auto pt: mUiViewMaskNormPoints[id]) {
                auto w = 1.0 / mUiViewMaskSize;
                auto h = 1.0 / mUiViewMaskSize;
                auto viewRect = QRectF(pt.x.toDouble(),pt.y.toDouble(),w,h);
                //LOG<<mapToSize(viewRect,p11,diameter,diameter);
                painter.fillRect(mapToSize(viewRect,p11,diameter,diameter),groupcolor);
        }

        painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
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
        pen.setWidth(2);
        pen.setColor(Qt::black);
        painter.setPen(pen);

        if (!mDrapRectF.isEmpty()) {
            auto pen = painter.pen();
            pen.setColor(Qt::blue);
            painter.setPen(pen);
            painter.drawRect(mapToSize(mDrapRectF,getInnerRectTopLeftPoint(),radius*2,radius*2));
            pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
            painter.setPen(pen);
        }

        event->accept();
    }
} // end namespace v2

namespace V1 {
    void ViewPattern::drawSelectRect(QPainter &painter)
    { // 根据当前孔的组颜色去绘制视野已经被选择的小矩形区域

        // 计算当前孔的唯一id
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
        auto groupcolor = mCurrentViewInfo[HoleGroupColorField].toString();
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx


        if (mViewSelectPoints[idx].isEmpty()) return; // 删孔后清除缓存信息这里是空直接返回否则遍历越界
        auto  rects = getInnerRects();
        // 绘制框选的所有视野
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                auto rect = rects[row][col];
                if (mViewSelectPoints[idx][row][col] && !mDisablePoints[row][col]) { // 多层保护,不可选的视野不允许绘制
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

        // 绘制框选的所有视野
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                if (mDrapPoints[row][col] && !mDisablePoints[row][col]) { // 多层保护,不可选的视野不允许绘制
                    auto rect = rects[row][col];
                    painter.fillRect(rect,mMouseClickColor);
                }
            }
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
        // rects.size =  4  mouse =  QPoint(7,6)
        // 原来10x10,鼠标可能点的(7,6),现在切换尺寸4x4,导致绘制鼠标的高亮越界! 所以初始化时必须重置鼠标
        if (mMousePoint != QPoint(-1,-1)) // 尚未点击
            painter.fillRect(rects[mMousePoint.x()][mMousePoint.y()],mMouseClickColor);
    }

    void ViewPattern::drawCircle(QPainter &painter)
    {// 画窗口的内接圆
        auto radius = getCircleRadius();
        painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
    }

    void ViewPattern::drawDisbaleRect(QPainter &painter)
    {
        auto  rects = getInnerRects();
        // 绘制置灰的所有视野
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                if (mDisablePoints[row][col]) {
                    auto rect = rects[row][col];
                    painter.fillRect(rect,mGrayColor);
                    painter.drawLine(rect.topLeft(),rect.bottomRight());
                    painter.drawLine(rect.topRight(),rect.bottomLeft());
                }
            }
        }
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


        drawDrapRect(painter); // 1.绘制拖拽区域最前
        drawSelectRect(painter); // 2. 被选中的高亮其次,要覆盖拖拽区域
        drawDisbaleRect(painter); // 3. 置灰区域覆盖被选中的区域再其次
        drawCircle(painter); // 4. 画线画圆防止被覆盖,最后
        drawInnerLine(painter);
        // 绘制框,最后画防止被覆盖
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