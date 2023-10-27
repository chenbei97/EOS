/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 08:22:41
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 08:25:21
 * @FilePath: \EOS\interface\src\protocol\protocolpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "protocolpattern.h"

ProtocolPattern::ProtocolPattern(int rows, int cols,QWidget*parent): Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    initHoleInfo();
    initDrapPoints();
}

void ProtocolPattern::setPatternSize(int rows, int cols)
{
    Pattern::setPatternSize(rows,cols);
    initHoleInfo();
    initDrapPoints();
}

void ProtocolPattern::paintEvent(QPaintEvent *event)
{
    Pattern::paintEvent(event);

    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();
    int radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内

    auto  centerPts = getCenterPoints();
    QPainterPath path;
    QPainter painter(this);

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto center = centerPts[row][col];

            // 启用了鼠标事件mMousePos才会被赋值,否则是(-1,-1),所以这里不用再判断是否启用了鼠标事件
            if (mMousePos.x() == row && mMousePos.y() == col && !mHoleInfo[row][col].isSelected)
            { // 已绘制的点不要绘制鼠标选中高亮
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,mMouseClickColor);
            }

            if (!mDrapRect.isNull()) { // 绘制框选框
                auto pen = painter.pen();
                pen.setColor(Qt::blue);
                painter.setPen(pen);
                painter.drawRect(mDrapRect);
                pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
                painter.setPen(pen);
            }

            // 绘制框选的所有点
            if (mDrapPoints[row][col]) { // && !mHoleInfo[row][col].isSelected 绘制拖拽临时的点,如果有已被选中的不需要再绘制
                path.clear(); // 不过这样取消选中时不能绘制拖拽的点感受不好,还是恢复
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, mMouseClickColor);
            }

            // 绘制确实选中的点
            if (mHoleInfo[row][col].isSelected) //  绘制确定选中的点
            {
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,mHoleInfo[row][col].color);
            }
        }
    }

    event->accept();
}

void ProtocolPattern::mouseMoveEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        if (event->buttons() & Qt::LeftButton) {
            initDrapPoints();

            auto end = event->pos(); // 鼠标停下的点
            mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
            auto rects = getChildRects();
            for(int row = 0; row < mrows; ++row)
                for(int col = 0; col < mcols; ++col) {
                    if(mDrapRect.intersects(rects[row][col])){ // 小矩形区域在这个推拽区域内有交集
                        mDrapPoints[row][col] = true;
                    }
                }
            //LOG<<"mDrapPoints is init";
        }
    }
    update();
    event->accept();
}

void ProtocolPattern::mouseReleaseEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        //LOG<<"mouse point = "<<mMousePos;
        if (mMousePos == QPoint(-1,-1))
            return; // 可能会点到边缘位置导致下方获取颜色越界

        int count = drapPointCount();
        //LOG<<"drap point count = "<<count;
        if (count <= 1) // 只框选了1个不会弹
            return;

        if (event->button() == Qt::LeftButton) {

            //这些代码转移到外部去写,发射信号即可,把select改为公共函数即可,对于GroupInfo目前只需要传递颜色而已
            auto c = mHoleInfo[mMousePos.x()][mMousePos.y()].color; // 选中点的颜色
            emit drapEvent(c);
//
//            auto dlg = new GroupInfo;
//            dlg->setBtnColor(c); // 鼠标单击时可以让按钮跟随当前的孔颜色
//
//            int ret = dlg->exec();
//            if (ret == QDialog::Accepted) {
//                select(dlg->groupColor());
//            }
//            delete dlg;
            mDrapRect.setWidth(0); // 清除矩形
            mDrapRect.setHeight(0);
        }
    }
    event->accept();
}

void ProtocolPattern::mousePressEvent(QMouseEvent *event)
{
    Pattern::mousePressEvent(event);
    initDrapPoints(); // 框选后，如果点一下应该取消框选
    mDrapRect.setWidth(0);
    mDrapRect.setHeight(0);
    update();
    event->accept();
}


void ProtocolPattern::initHoleInfo()
{
    mHoleInfo.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QHoleInfoVector var;
        for (int col = 0; col < mcols; ++col){
            HoleInfo info;
            info.point = QPoint(row,col);
            info.color = Qt::red;
            info.isSelected = false;
            var.append(info);
        }
        mHoleInfo.append(var);
    }
    update();
}

void ProtocolPattern::initDrapPoints()
{
    mDrapPoints.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QBoolVector var;
        for (int col = 0; col < mcols; ++col){
            var.append(false);
        }
        mDrapPoints.append(var);
    }
    update();
}

int ProtocolPattern::drapPointCount() const
{
    int count = 0;
    for(int r = 0; r < mrows; ++ r) {
        for(int c = 0; c < mcols; ++c) {
            if (mDrapPoints[r][c])
                count++;
        }
    }
    return count;
}

void ProtocolPattern::select(QCColor color)
{// 把拖拽的点都选中
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto pt = mDrapPoints[row][col];
            if (pt){
                mHoleInfo[row][col].isSelected = true;//框选内对应的点都设为选中
                mHoleInfo[row][col].color = color; // 颜色跟随窗口设置的颜色
                mDrapPoints[row][col] = false; // 拖拽区域内的点也要更新为false,不然还会绘制这个区域
            }
        }
    }
    auto ret = mMousePos.x()<0 || mMousePos.x()>mrows-1
               || mMousePos.y()<0 || mMousePos.y()>mcols-1;
    if (!ret) {// 防止越界,选择和取消选择需要在未初始化时禁用动作,
        mHoleInfo[mMousePos.x()][mMousePos.y()].isSelected = true; // 没启用鼠标事件,这是{-1,-1}会越界
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = color;
        mDrapPoints[mMousePos.x()][mMousePos.y()] = false;
    }
    update();
}