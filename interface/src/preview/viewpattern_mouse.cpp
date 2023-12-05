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
{
    mMousePos = event->pos();
    if (!getValidRect().contains(mMousePos)) // 如果不是有效区域的鼠标点击无效
        mMousePos = {-1.0,-1.0};

    // 1.框选后随机点一下要清除框选框(只能拖拽生成),同时出现单击框
    if (event->button() == Qt::LeftButton) {
        mDrapRectF = QRectF();

        auto diameter = width()>=height()?height():width();
        auto mouseRect_topleft = mMousePos-QPointF(getInnerRectWidth()/2,getInnerRectHeight()/2);
        auto mouseRect_bottomright = mMousePos+QPointF(getInnerRectWidth()/2,getInnerRectHeight()/2);
        if (getValidRect().contains(mouseRect_topleft) && getValidRect().contains(mouseRect_topleft)) {
            mMouseRect = mapFromSize(QRectF(mouseRect_topleft,mouseRect_bottomright),
                                     getInnerRectTopLeftPoint(),diameter,diameter);
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        if (mViewInfo[HoleGroupNameField].toString().isEmpty()
            || mMousePos==QPointF(-1.0,-1.0)) {
            applyallact->setEnabled(false);
            applygroupact->setEnabled(false);
            saveviewact->setEnabled(false);
            removeviewact->setEnabled(false);
        } else {
            applyallact->setEnabled(true);
            applygroupact->setEnabled(true);
            saveviewact->setEnabled(true);
            removeviewact->setEnabled(true);

//                auto rectinfo = mViewRects[holeID()];
//                if (rectinfo.isEmpty()) {
//                    removeviewact->setEnabled(false);
//                } else {
//                    for(auto viewRect: rectinfo) {
//                        if (viewRect.flag) { // 保存的区域
//                            if (viewRect.rect.contains(mDrapRectF)) { // 已经包含了
//                                saveviewact->setEnabled(false);
//                                removeviewact->setEnabled(true);
//                            } else if (!viewRect.rect.intersects(mDrapRectF)){ // 无交集
//                                saveviewact->setEnabled(true); // 不是已经保存的可以保存
//                                removeviewact->setEnabled(true);
//                            } else { // 有交集
//                                saveviewact->setEnabled(true);
//                                removeviewact->setEnabled(true);
//                            }
//                        } else {
//                            if (viewRect.rect.contains(mDrapRectF)) { // 已经是完全删除的
//                                saveviewact->setEnabled(true); //可以保存但是不可以删
//                                removeviewact->setEnabled(false);
//                            } else if (!viewRect.rect.intersects(mDrapRectF)){ // 不是删除的
//                                saveviewact->setEnabled(false);
//                                removeviewact->setEnabled(true);// 不是已经删除的去删除
//                            } else {
//                                saveviewact->setEnabled(true);
//                                removeviewact->setEnabled(true);
//                            }
//                        }
//                    }
//                }
        }


    }
    // 将鼠标坐标映射为相对圆外接正方形左上角的相对坐标,并将其归一化
    if (mMousePos != QPointF(-1.0,-1.0)) {
        auto pos = mapFromPointF(mMousePos);
        emit previewEvent(pos);
    }
    update();
    event->accept();
}

void ViewPattern::mouseReleaseEvent(QMouseEvent *event)
{
//        if (!mDrapRectF.isEmpty()) {
//            mMouseRect = QRectF();
//        }
//        update();
//        event->accept();
}

void ViewPattern::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        auto end = event->pos(); // 鼠标停下的点
        if (!getValidRect().contains(mMousePos) || !getValidRect().contains(end))
            return; // 框选到无效区域
        mMouseRect = QRectF();
        auto diameter = width()>=height()?height():width();
        // 鼠标形成的矩形框将其等比例缩放到0-1
        mDrapRectF = mapFromSize(QRectF(mMousePos,end),
                                 getInnerRectTopLeftPoint(),diameter,diameter);
    }
    update();
    event->accept();
}