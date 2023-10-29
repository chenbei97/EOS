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


