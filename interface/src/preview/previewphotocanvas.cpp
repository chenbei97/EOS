/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\photocanvas.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewphotocanvas.h"

void PreviewPhotoCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    // 绘制框
    if (!mDrapRect.isNull()) {
        auto pen = painter.pen();
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawRect(mDrapRect);
        pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
        painter.setPen(pen);
    }

    if (strategy == NoStrategy) {
        event->accept();
        return;
    }

    // 其他策略
    event->accept();
}

void PreviewPhotoCanvas::mousePressEvent(QMouseEvent *event)
{ // 左键点击清除框选,计算鼠标点击的小矩形区域坐标
    if (event->button() == Qt::LeftButton) {
        mDrapRect.setWidth(0);
        mDrapRect.setHeight(0);
        mDrapRect = QRectF();
        update();
    } // 右键是菜单
    mLastPos = event->pos();
    //mMousePoint = {-1,-1};
    //emit mouseClicked(mMousePoint);
    event->accept();
    update();
}

void PreviewPhotoCanvas::mouseMoveEvent(QMouseEvent *event)
{ // 绘制拖拽框
    if (event->buttons() & Qt::LeftButton){
        auto end = event->pos(); // 鼠标停下的点
        mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
    }
    update();
    event->accept();
}

void PreviewPhotoCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
}


void PreviewPhotoCanvas::setStrategy(PreviewPhotoCanvas::DrawStrategy s, const QVariantMap& m)
{ //
    // 1. 初始化通用信息
    strategy = s;
    mMousePoint = {-1,-1}; // 必须更新,否则上次的鼠标点还在会导致切换物镜或者brand出现越界
    mLastPos = {-1,-1};
    mDrapRect = QRectF(); // 上次的框选痕迹还在要清除

    // 2. 初始化xx策略需要的信息
    mCurrentHoleInfo = m;

    update();
}



QVariantMap PreviewPhotoCanvas::currentHoleInfo() const
{ //
    return mCurrentHoleInfo;
}

PreviewPhotoCanvas::PreviewPhotoCanvas(QWidget *parent) : QWidget(parent)
{
    strategy = NoStrategy;
    mrows = 0;
    mcols = 0;
    mMousePoint = QPoint(-1,-1);
    mLastPos = {-1,-1};
    mMouseClickColor.setAlpha(PatternColorAlpha);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}
