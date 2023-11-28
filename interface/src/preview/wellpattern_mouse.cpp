/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\wellpattern_mouse.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "wellpattern.h"

namespace V1 {
    void WellPattern::mouseReleaseEvent(QMouseEvent *event)
    { // 框选数量大于1不允许打开视野窗口,只能基于特定孔
        if (mMouseEvent) {

            // 1. 点到边缘位置不能打开视野窗口和设置分组信息
            if (mMousePos == QPoint(-1,-1)){
                openviewact->setEnabled(false);
                setgroupact->setEnabled(false);
                removeholeact->setEnabled(false);
                return;
            }

            // 2.点到不可选的孔也不能打开
            if (mDisablePoints[mMousePos.x()][mMousePos.y()]) {
                openviewact->setEnabled(false);
                setgroupact->setEnabled(false);
                removeholeact->setEnabled(false);
                return;
            }

            // 3. 点到其它可用的孔且非边缘位置就可以打开分组窗口
            setgroupact->setEnabled(true);
            removeholeact->setEnabled(true);

            // 4. 框选的数量大于1个不允许打开视野动作使能,打开视野只能基于特定孔打开
            int count = drapPointCount();
            if (count > 1) // 只框选了1个不会弹
                openviewact->setEnabled(false);
            else openviewact->setEnabled(true);
        }
        event->accept();
    }

    void WellPattern::mouseMoveEvent(QMouseEvent *event)
    {// 得到框选孔的区域
        if (mMouseEvent) {
            if (event->buttons() & Qt::LeftButton)
            {
                initDrapPoints(); // 清除拖拽区域
                auto end = event->pos(); // 鼠标停下的点
                mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
                auto rects = getChildRects();
                for(int row = 0; row < mrows; ++row)
                    for(int col = 0; col < mcols; ++col) {
                        if(mDrapRect.intersects(rects[row][col])// 小矩形区域在这个推拽区域内有交集
                           && !mDisablePoints[row][col]){ // 根源在mDrapPoints直接限制好,paintEvent不加
                            mDrapPoints[row][col] = true; //!mDisablePoints[row][col]判断也可以
                        }
                    }

            }
        }
        update();
        event->accept();
    }

    void WellPattern::mousePressEvent(QMouseEvent *event)
    { // 点左键要取消框选
        Pattern::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {
            initDrapPoints(); // 框选后，如果左键点一下应该取消框选
            mDrapRect.setWidth(0);
            mDrapRect.setHeight(0);
            mDrapRect = QRectF();
            update();
        } // 右键是菜单

        event->accept();
    }

    void WellPattern::mouseDoubleClickEvent(QMouseEvent*event)
    { // 双击也能打开视野窗口
        Pattern::mouseDoubleClickEvent(event);

        // 点到不可选的孔也不能打开,这样wellpattern的限制就禁止了viewpattern的打开,viewpattern不再需要针对置灰功能写逻辑
        if (mDisablePoints[mMousePos.x()][mMousePos.y()]) {
            openviewact->setEnabled(false);
            setgroupact->setEnabled(false);
            removeholeact->setEnabled(false);
            return;
        }
        openviewact->setEnabled(true);
        setgroupact->setEnabled(true);
        removeholeact->setEnabled(true);
        openviewact->trigger(); // 触发一下
    }
}// end namespace v1