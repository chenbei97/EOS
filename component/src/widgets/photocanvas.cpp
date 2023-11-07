/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\photocanvas.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "photocanvas.h"

void PhotoCanvas::paintEvent(QPaintEvent *event)
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

    switch (strategy) {
        case NoStrategy:
            break;
        case SinglePixmap:
            auto image = mStrategyInfo[ImageField].value<QImage>();
            if(image.isNull()) return;
            auto img = image.scaled(width(),height(),Qt::KeepAspectRatio,Qt::FastTransformation);
            auto targetRect = QRectF(0,0,width(),height());
            auto sourceRect = targetRect;
            painter.drawImage(targetRect,image,sourceRect);
            break;
    }
    event->accept();
}

void PhotoCanvas::mousePressEvent(QMouseEvent *event)
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

void PhotoCanvas::mouseMoveEvent(QMouseEvent *event)
{ // 绘制拖拽框
    if (event->buttons() & Qt::LeftButton){
        auto end = event->pos(); // 鼠标停下的点
        mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
    }
    update();
    event->accept();
}

void PhotoCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
}

void PhotoCanvas::setStrategy(PhotoCanvas::DrawStrategy s, const QVariantMap& m)
{ //
    // 1. 初始化通用信息
    strategy = s;
    mMousePoint = {-1,-1};
    mLastPos = {-1,-1};
    mDrapRect = QRectF();

    // 2. 初始化xx策略需要的信息
    mStrategyInfo = m;

    update();
}

void PhotoCanvas::setData(const QVariantMap &m)
{
    mStrategyInfo = m;
    update();
}

PhotoCanvas::PhotoCanvas(QWidget *parent) : QWidget(parent)
{
    strategy = NoStrategy;
    mMousePoint = QPoint(-1,-1);
    mLastPos = {-1,-1};
    mMouseClickColor.setAlpha(DefaultColorAlpha);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}
