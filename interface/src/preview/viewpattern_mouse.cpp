/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\viewpattern_mouse.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

namespace V2 {
    void ViewPattern::mousePressEvent(QMouseEvent *event)
    {

    }

    void ViewPattern::mouseReleaseEvent(QMouseEvent *event)
    {

    }

    void ViewPattern::mouseMoveEvent(QMouseEvent *event)
    {

    }
}

namespace V1 {
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
                if (rects[r][c].contains(mLastPos) && !mDisablePoints[r][c]) {
                    mMousePoint = {r,c}; // 多层保护,不可选的视野视为-1,-1,这样去设置使能 ①,②做法均可
                }
            }
        }
//    if (mMousePoint != QPoint(-1,-1)) {
//        if (mDisablePoints[mMousePoint.x()][mMousePoint.y()])
//            mMousePoint = {-1,-1}; // 多层保护,不可选的视野视为-1,-1,这样去设置使能,①,②做法均可
//    }

        // 右键时如果这个视野没被选择过不能删除点
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();//当前视野窗口隶属的孔坐标
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        auto viewpoints = mViewSelectPoints[idx];// 拿到本孔对应的视野坐标
        int viewcount = viewPointCount(); // 本孔选择的视野数量

        auto groupname = mCurrentViewInfo[HoleGroupNameField].toString();
        if (!groupname.isEmpty() && viewcount)
            applygroupact->setEnabled(true);
        else applygroupact->setEnabled(false);//如果视野数量=0或者本孔未分组不能应用到本组

        // 视野数量=0不能应用到所有组
        viewcount?applyallact->setEnabled(true):applyallact->setEnabled(false);
        // 视野等于=0不能删点,这里改为当前鼠标右击的视野坐标是否已被选择过也可以
        bool isselect = false;
        if (mMousePoint != QPoint(-1,-1)) // 防止下方索引越界,如果是可选的视野坐标再去判断是否被选中
            isselect = viewpoints[mMousePoint.x()][mMousePoint.y()];
        isselect?removeviewact->setEnabled(true):removeviewact->setEnabled(false);

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
                    if(mDrapRect.intersects(rects[row][col]) // 小矩形区域在这个推拽区域内有交集
                       && !mDisablePoints[row][col]){ //根源在mDrapPoints直接限制好,paintEvent不加
                        mDrapPoints[row][col] = true;//!mDisablePoints[row][col]判断也可以
                    }
                }
        }
        update();
        event->accept();
    }

    void ViewPattern::mouseReleaseEvent(QMouseEvent *event)
    { // 拖拽区域点个数为0才是预览事件(这里不能对applyall,applygroup,remove的使能操作)

        // 1. 点到边缘不能保存点
        if (mMousePoint == QPoint(-1,-1) ){ // 右击不触发
            saveviewact->setEnabled(false);
            removeviewact->setEnabled(false);
            applyholeact->setEnabled(false);
            applygroupact->setEnabled(false);
            applyallact->setEnabled(false);
            return; // 可能会点到边缘位置,不能选点,由于绘图没有死区,这不设置也行
        }

        // 2. 点到不能选的孔也不能保存点
        if (mDisablePoints[mMousePoint.x()][mMousePoint.y()]) {
            saveviewact->setEnabled(false);
            removeviewact->setEnabled(false);
            applyholeact->setEnabled(false);
            applygroupact->setEnabled(false);
            applyallact->setEnabled(false);
            return;
        }

        saveviewact->setEnabled(true);
        removeviewact->setEnabled(true);
        applyholeact->setEnabled(true);
        applygroupact->setEnabled(true);
        applyallact->setEnabled(true);

        if (drapPointCount()<1 ) // 没有拖拽区域单击才是预览事件
        {
            LOG<<"preview event is emit";
            emit previewEvent(mMousePoint);
        }

        event->accept();
    }
} // end namespace v1