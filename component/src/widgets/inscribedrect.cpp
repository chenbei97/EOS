/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 13:28:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 13:28:29
 * @FilePath: \EOS\component\src\widgets\inscribedrect.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "inscribedrect.h"

void InscribedRect::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    drawCircle(painter);
    drawInnerRect(painter);
    fillInnerRect(painter);

    // 绘制框
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

void InscribedRect::fillInnerRect(QPainter &painter)
{// 高亮圆内接的小矩形,彼此有间隔,不是连续的
    // 1. 方法1
//    auto rects = getInnerRectsVector();
//    for(int r = 0; r < mrows; ++r) {
//        for(int c = 0; c < mcols; ++c) {// 一维映射
//            painter.fillRect(rects[r*mcols+c],InscribedRectColor);
//        }
//    }
    // 2. 方法2
//    foreach(auto rect,rects)
//                painter.fillRect(rect,InscribedRectColor);
    // 3. 方法3
    auto rects2d = getInnerRects2DVector();
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {// 一维映射
            painter.fillRect(rects2d[r][c],InscribedRectColor);
        }
    }
}

void InscribedRect::drawCircle(QPainter &painter)
{// 画窗口的内接圆
    auto radius = getCircleRadius();
    painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
}

void InscribedRect::drawInnerRect(QPainter &painter)
{
    auto size = getInnerRectSize();
    painter.drawRect(QRectF(getInnerRectTopLeftPoint(),QSize(size,size)));
}

void InscribedRect::mousePressEvent(QMouseEvent *event)
{ // 左键点击清除框选,计算鼠标点击的小矩形区域坐标
    if (event->button() == Qt::LeftButton) {
        mDrapRect.setWidth(0);
        mDrapRect.setHeight(0);
        mDrapRect = QRectF();
        update();
    } // 右键是菜单

    mLastPos = event->pos();
    mMousePoint = {-1,-1};
    auto rects = getInnerRects2DVector();
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            if (rects[r][c].contains(mLastPos)) {
                mMousePoint = {r,c};
            }
        }
    }
    //LOG<<"mouse point = "<<mMousePoint;
    emit mouseClicked(mMousePoint);
    event->accept();
    update();
}

void InscribedRect::mouseMoveEvent(QMouseEvent *event)
{ // 绘制拖拽框
    if (event->buttons() & Qt::LeftButton){
        auto end = event->pos(); // 鼠标停下的点
        mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
    }
    update();
    event->accept();
}

void InscribedRect::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
}

double InscribedRect::getCircleRadius() const
{ // 视野圆半径
    return width()>=height()?height()/2.0:width()/2.0;
}

double InscribedRect::getInnerRectSize() const
{ // 计算圆内接正方形的尺寸
    return getCircleRadius()* sqrt(2);
}

QPointF InscribedRect::getInnerRectTopLeftPoint() const
{ // 窗口内有个圆,圆的内部有个内接正方形,正方形的左上角顶点 (w/2-r*sqrt(2)/2,r-r*sqrt(2)/2)
    auto radius = getCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标为窗口的一半减去半径的45°对边
    auto y = radius - radius * sqrt(2)/2.0; // y坐标为半径减去半径的45°对边

    return QPointF(x,y);
}

QPointF InscribedRect::getInnerRectTopRightPoint() const
{ // 窗口内有个圆,圆的内部有个内接正方形,正方形的右上角顶点 (w/2+r*sqrt(2)/2,r-r*sqrt(2)/2)
    auto radius = getCircleRadius();
    auto x = width()/2.0 + radius * sqrt(2)/2.0; // x坐标为窗口的一半加上半径的45°对边
    auto y = radius - radius * sqrt(2)/2.0; // y坐标和左上角相同

    return QPointF(x,y);
}

QPointF InscribedRect::getInnerRectBottomLeftPoint() const
{// 窗口内有个圆,圆的内部有个内接正方形,正方形的左下角顶点 (w/2-r*sqrt(2)/2,r+r*sqrt(2)/2)
    auto radius = getCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标和左上角相同
    auto y = radius + radius * sqrt(2)/2.0; // y坐标为半径加上半径的45°对边

    return QPointF(x,y);
}

QPointF InscribedRect::getInnerRectBottomRightPoint() const
{// 窗口内有个圆,圆的内部有个内接正方形,正方形的右下角顶点 (w/2+r*sqrt(2)/2,r+r*sqrt(2)/2)
    auto radius = getCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标和右上角相同
    auto y = radius + radius * sqrt(2)/2.0; // y坐标和左下角相同

    return QPointF(x,y);
}

QRectFVector InscribedRect::getInnerRectsVector() const
{ //获取圆内接正方形内的所有小正方形区域
    QRectFVector vec;
    auto rect_size = getInnerRectSize(); // 内接正方形的尺寸,在这个尺寸内计算水平和垂直间距

    auto hor_gap = (rect_size-mcols * mInnerRectSize) / (mcols+1.0); // 小矩形之间的水平间距
    auto ver_gap = (rect_size-mrows * mInnerRectSize) / (mrows+1.0); // 小矩形之间的垂直间距

    // 水平垂直偏移从正方形左上角顶点开始
    auto start_x = getInnerRectTopLeftPoint().x();
    auto start_y = getInnerRectTopLeftPoint().y();

    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            auto x = hor_gap + c * (mInnerRectSize+hor_gap) + start_x; // 水平距离用c *
            auto y = ver_gap + r * (mInnerRectSize+ver_gap) + start_y;// 垂直距离用r *
            auto rect = QRect(x,y,mInnerRectSize,mInnerRectSize);
            vec.append(rect);
        }
    }
    return vec;
}

QRectF2DVector InscribedRect::getInnerRects2DVector() const
{ //获取圆内接正方形内的所有小正方形区域
    QRectF2DVector vec;
    auto rect_size = getInnerRectSize(); // 内接正方形的尺寸,在这个尺寸内计算水平和垂直间距

    auto hor_gap = (rect_size-mcols * mInnerRectSize) / (mcols+1.0); // 小矩形之间的水平间距
    auto ver_gap = (rect_size-mrows * mInnerRectSize) / (mrows+1.0); // 小矩形之间的垂直间距

    // 水平垂直偏移从正方形左上角顶点开始
    auto start_x = getInnerRectTopLeftPoint().x();
    auto start_y = getInnerRectTopLeftPoint().y();

    for(int r = 0; r < mrows; ++r) {
        QRectFVector v;
        for(int c = 0; c < mcols; ++c) {
            auto x = hor_gap + c * (mInnerRectSize+hor_gap) + start_x; // 水平距离用c *
            auto y = ver_gap + r * (mInnerRectSize+ver_gap) + start_y;// 垂直距离用r *
            auto rect = QRect(x,y,mInnerRectSize,mInnerRectSize);
            v.append(rect);
        }
        vec.append(v);
    }
    return vec;
}

void InscribedRect::setInnerRectSize(int size)
{// 圆内接正方形 小方格的尺寸 可能默认的会太大或者太小,外部可以调整
    mInnerRectSize = size;
}

InscribedRect::InscribedRect(QWidget *parent) : QWidget(parent)
{
    mrows = InscribedRectDefaultRows;
    mcols = InscribedRectDefaultCols;
    mMousePoint = QPoint(-1,-1);
    mLastPos = {-1,-1};
    mMouseClickColor.setAlpha(DefaultColorAlpha);
    mDrapRect = QRectF();
    mInnerRectSize = InscribedRectSize;
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

InscribedRect::InscribedRect(int row,int col,QWidget *parent) : QWidget(parent)
{
    mrows = row;
    mcols = col;
    mMousePoint = QPoint(-1,-1);
    mLastPos = {-1,-1};
    mMouseClickColor.setAlpha(DefaultColorAlpha);
    mDrapRect = QRectF();
    mInnerRectSize = InscribedRectSize;
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void InscribedRect::setPatternSize(int row, int col)
{
    mrows = row;
    mcols = col;
    mMousePoint = QPoint(-1,-1);
    mLastPos = {-1,-1};
    mDrapRect = QRectF();
    mInnerRectSize = InscribedRectSize;
    update();
}