/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:53:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-27 08:54:02
 * @FilePath: \EOS\interface\src\preview\wellpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

void WellPattern::paintEvent(QPaintEvent *event)
{
    Pattern::paintEvent(event);

    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();
    int radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内
    auto  centerPts = getCenterPoints();

    QPainter painter(this);
    QPainterPath path;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto center = centerPts[row][col];

            // (1) 绘制鼠标点击的高亮
            //启用了鼠标事件mMousePos才会被赋值,否则是(-1,-1),所以这里不用再判断是否启用了鼠标事件
            if (mMousePos.x() == row && mMousePos.y() == col && !mHoleInfo[row][col].isselected) {
                path.clear(); // 已绘制的点不要绘制鼠标选中高亮
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, mMouseClickColor);
            }

            // (2) 绘制框选的所有孔
            if (mDrapPoints[row][col]) { // && !mHoleInfo[row][col].isSelected 绘制拖拽临时的点,如果有已被选中的不需要再绘制
                path.clear(); // 不过这样取消选中时不能绘制拖拽的点感受不好,还是恢复
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, mMouseClickColor);
            }

            // (3) 绘制确实选中的孔
            if (mHoleInfo[row][col].isselected) //  绘制确定选中的点
            {
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,mHoleInfo[row][col].color);
            }
        }
    }
    // (4) 绘制框选框(放最后画,防止被填充颜色覆盖)
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

void WellPattern::mouseReleaseEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        //LOG<<"mouse point = "<<mMousePos;
        if (mMousePos == QPoint(-1,-1)){
            viewact->setEnabled(false);
            groupact->setEnabled(false);
            return; // 可能会点到边缘位置
        }

        groupact->setEnabled(true);

        int count = drapPointCount();
        if (count > 1) // 只框选了1个不会弹
            viewact->setEnabled(false);
        else viewact->setEnabled(true);
        //emit drapEvent(mHoleInfo[mMousePos.x()][mMousePos.y()].color);

//        if (event->button() == Qt::LeftButton) {
//            mDrapRect.setWidth(0); // 清除拖拽矩形(不需要了,pressevent是左键就会清除了)
//            mDrapRect.setHeight(0);
//            mDrapRect = QRectF();
//        }
    }
    event->accept();
}

void WellPattern::mouseMoveEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        if (event->buttons() & Qt::LeftButton)
        {
            initDrapPoints(); // 清除拖拽区域
            auto end = event->pos(); // 鼠标停下的点
            mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
            auto rects = getChildRects();
            for(int row = 0; row < mrows; ++row)
                for(int col = 0; col < mcols; ++col) {
                    if(mDrapRect.intersects(rects[row][col])){ // 小矩形区域在这个推拽区域内有交集
                        mDrapPoints[row][col] = true;
                    }
                }

        }
    }
    update();
    event->accept();
}

void WellPattern::mousePressEvent(QMouseEvent *event)
{
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

void WellPattern::setGroup(QCVariantMap m)
{
    auto gcolor = m[GroupColorField].toString();
    auto gname = m[GroupNameField].toString();
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto pt = mDrapPoints[row][col]; // 所有拖拽的点都分为一组
            if (pt){
                mHoleInfo[row][col].isselected = true;//框选内对应的点都设为选中
                mHoleInfo[row][col].color = gcolor; // 颜色跟随窗口设置的颜色
                mHoleInfo[row][col].group = gname;
                mDrapPoints[row][col] = false; // 拖拽区域内的点也要更新为false,不然还会绘制这个区域
            }
        }
    }

    // 框选的时候会遗漏鼠标当前选中的点
    if (mMousePos != QPoint(-1,-1)) {// 没启用鼠标事件或者点击外围,这是{-1,-1}会越界
        mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = true; // 鼠标点击的这个
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = gcolor;
        mHoleInfo[mMousePos.x()][mMousePos.y()].group = gname;
        mDrapPoints[mMousePos.x()][mMousePos.y()] = false;
    }

    LOG<<"mHoleInfo = "<<mHoleInfo[mMousePos.x()][mMousePos.y()];
    update();
}

void WellPattern::initDrapPoints()
{ // 拖拽结束后清除这些点
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

int WellPattern::drapPointCount() const
{ // 计算拖拽区域包含的点个数
    int count = 0;
    for(int r = 0; r < mrows; ++ r) {
        for(int c = 0; c < mcols; ++c) {
            if (mDrapPoints[r][c])
                count++;
        }
    }
    return count;
}

void WellPattern::initHoleInfo()
{ // 初始化所有孔内的包含信息
    mHoleInfo.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QHoleInfoVector var;
        for (int col = 0; col < mcols; ++col){
            HoleInfo info;
            info.coordinate = QPoint(row,col);
            info.color = Qt::red;
            info.isselected = false;
            var.append(info);
        }
        mHoleInfo.append(var);
    }
    update();
}

void WellPattern::onSetViewAct()
{
    // dosomething
    if (mMousePos != QPoint(-1,-1))
        emit viewEvent(mMousePos);
}

void WellPattern::onSetGroupAct()
{
    auto color = mHoleInfo[mMousePos.x()][mMousePos.y()].color;
    auto name = mHoleInfo[mMousePos.x()][mMousePos.y()].group;
    QVariantMap m;
    m[GroupColorField] = color;
    m[GroupNameField] = name;
    emit drapEvent(m);
}

WellPattern::WellPattern(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    groupact = new QAction(tr("分组"));
    viewact = new QAction(tr("选点"));
    addAction(groupact);
    addAction(viewact);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    initDrapPoints();
    initHoleInfo();

    connect(groupact,&QAction::triggered,this,&WellPattern::onSetGroupAct);
    connect(viewact,&QAction::triggered,this,&WellPattern::onSetViewAct);
}

void WellPattern::setPatternSize(int rows, int cols)
{
    Pattern::setPatternSize(rows,cols);
    initDrapPoints();
    initHoleInfo();
}