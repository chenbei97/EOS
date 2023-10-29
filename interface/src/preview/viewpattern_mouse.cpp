/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\viewpattern_mouse.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

void ViewPattern::mousePressEvent(QMouseEvent *event)
{ // 左键点击清除框选,计算鼠标点击的小矩形区域坐标
    if (event->button() == Qt::LeftButton) {
        initDrapPoints(); // 框选后，如果左键点一下应该取消框选
        mDrapRect.setWidth(0);
        mDrapRect.setHeight(0);
        mDrapRect = QRectF();
        update();
    } // 右键是菜单
    mLastPos = event->pos();
    mMousePoint = {-1,-1};
    auto rects = getInnerRects();
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            if (rects[r][c].contains(mLastPos)) {
                mMousePoint = {r,c};
            }
        }
    }
    update();
    emit mouseClicked(mMousePoint);
    event->accept();
}

void ViewPattern::mouseMoveEvent(QMouseEvent *event)
{ // 绘制拖拽框
    if (event->buttons() & Qt::LeftButton){
        initDrapPoints(); // 清除拖拽区域
        auto end = event->pos(); // 鼠标停下的点
        mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
        auto rects = getInnerRects();
        for(int row = 0; row < mrows; ++row)
            for(int col = 0; col < mcols; ++col) {
                if(mDrapRect.intersects(rects[row][col])){ // 小矩形区域在这个推拽区域内有交集
                    mDrapPoints[row][col] = true;
                }
            }

    }
    update();
    event->accept();
}

void ViewPattern::mouseReleaseEvent(QMouseEvent *event)
{ // 拖拽区域点个数为0才是预览事件
    if (mMousePoint == QPoint(-1,-1)){
        saveviewact->setEnabled(false);
        return; // 可能会点到边缘位置,不能选点,由于绘图没有死区,这不设置也行
    }
    saveviewact->setEnabled(true);

    if (drapPointCount()<1) // 没有拖拽区域单击才是预览事件
    {
        //LOG<<"preview event is emit";
        emit previewEvent(mMousePoint);
    }

    event->accept();
}


double ViewPattern::getExternalCircleRectSize() const
{ // 计算圆内接正方形的尺寸
    return getCircleRadius()* sqrt(2);
}

QPoint ViewPattern::getExternalCircleRectTopLeftPoint() const
{ // 窗口内有个圆,圆的内部有个内接正方形,正方形的左上角顶点 (w/2-r*sqrt(2)/2,r-r*sqrt(2)/2)
    auto radius = getCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标为窗口的一半减去半径的45°对边
    auto y = radius - radius * sqrt(2)/2.0; // y坐标为半径减去半径的45°对边

    return QPoint(x,y);
}

QPoint ViewPattern::getExternalCircleRectTopRightPoint() const
{ // 窗口内有个圆,圆的内部有个内接正方形,正方形的右上角顶点 (w/2+r*sqrt(2)/2,r-r*sqrt(2)/2)
    auto radius = getCircleRadius();
    auto x = width()/2.0 + radius * sqrt(2)/2.0; // x坐标为窗口的一半加上半径的45°对边
    auto y = radius - radius * sqrt(2)/2.0; // y坐标和左上角相同

    return QPoint(x,y);
}

QPoint ViewPattern::getExternalCircleRectBottomLeftPoint() const
{// 窗口内有个圆,圆的内部有个内接正方形,正方形的左下角顶点 (w/2-r*sqrt(2)/2,r+r*sqrt(2)/2)
    auto radius = getCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标和左上角相同
    auto y = radius + radius * sqrt(2)/2.0; // y坐标为半径加上半径的45°对边

    return QPoint(x,y);
}

QPoint ViewPattern::getExternalCircleRectBottomRightPoint() const
{// 窗口内有个圆,圆的内部有个内接正方形,正方形的右下角顶点 (w/2+r*sqrt(2)/2,r+r*sqrt(2)/2)
    auto radius = getCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标和右上角相同
    auto y = radius + radius * sqrt(2)/2.0; // y坐标和左下角相同

    return QPoint(x,y);
}

void ViewPattern::drawExternalCircleRect(QPainter &painter)
{// 高亮圆内接的小矩形,彼此有间隔,不是连续的
    auto rects = getExternalCircleRects();
    QPainterPath path;
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            path.clear();
            path.addRect(rects[r*mrows+c]);
            painter.fillPath(path,PreviewPhotoCanvasViewRectColor);
        }
    }

}

void ViewPattern::setExternalCircleRectSize(int size)
{// 圆内接正方形 小方格的尺寸 可能默认的会太大或者太小,外部可以调整
    mExternalCircleRectSize = size;
}

QRectVector ViewPattern::getExternalCircleRects() const
{ //获取圆内接正方形内的所有小正方形区域
    QRectVector vec;
    auto rect_size = getExternalCircleRectSize(); // 内接正方形的尺寸,在这个尺寸内计算水平和垂直间距

    auto hor_gap = (rect_size-mrows * mExternalCircleRectSize) / (mrows+1.0);
    auto ver_gap = (rect_size-mcols * mExternalCircleRectSize) / (mcols+1.0);

    // 水平垂直偏移从正方形左上角顶点开始
    auto hor_offset = getExternalCircleRectTopLeftPoint().x();
    auto ver_offset = getExternalCircleRectTopLeftPoint().y();

    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            auto x = hor_gap + r * (mExternalCircleRectSize+hor_gap) + hor_offset;
            auto y = ver_gap + c * (mExternalCircleRectSize+ver_gap) + ver_offset;
            auto rect = QRect(x,y,mExternalCircleRectSize,mExternalCircleRectSize);
            vec.append(rect);
        }
    }
    return vec;
}