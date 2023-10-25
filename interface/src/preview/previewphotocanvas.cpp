/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\photocanvas.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewphotocanvas.h"

PreviewPhotoCanvas::PreviewPhotoCanvas(QWidget *parent) : QWidget(parent)
{
    strategy = NoStrategy;
    mrows = 0;
    mcols = 0;
    mMousePoint = QPoint(-1,-1);
    mExternalCircleRectSize = DefaultPreviewPhotoCanvasViewRectSize;
    mMouseClickColor.setAlpha(PatternColorAlpha);
}

void PreviewPhotoCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    switch (strategy) {
        case ExternalCircleRect: // 内接圆
            drawExternalCircle(painter);
            drawExternalCircleRect(painter);
            break;
        case InnerCircleRect:
            drawExternalCircle(painter);
            drawInnerLine(painter);
            break;
        default:
            break;
    }
    event->accept();
}

void PreviewPhotoCanvas::mousePressEvent(QMouseEvent *event)
{
    auto pos = event->pos();
    mMousePoint = {-1,-1};
    auto rects = getInnerRects();
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            if (rects[r][c].contains(pos)) {
                mMousePoint = {r,c};
            }
        }
    }
    update();
    emit mouseClicked(mMousePoint);
    event->accept();
}

QRectF2DVector PreviewPhotoCanvas::getInnerRects() const
{
    QRectF2DVector m;
    auto hoffset = getInnerRectWidth();
    auto voffset = getInnerRectHeight();

    auto start = getInnerRectTopLeftPoint();
    for(int i = 0 ; i < mrows; ++i) {
        QRectFVector vec;
        for(int j = 0; j < mcols; ++j) { // j*offset加在x坐标也就是水平方向
            auto topleft = start + QPointF(j*hoffset,i*voffset); // x, x+d，j依次取0,1; y先不变在后
            auto bottomright = topleft + QPointF(hoffset,voffset);
            vec.append(QRectF(topleft,bottomright));
        }
        m.append(vec);
    }

    return m;
}

void PreviewPhotoCanvas::drawInnerLine(QPainter &painter)
{
    // 绘制外边框
    auto p11 = getInnerRectTopLeftPoint();
    auto p12 = getInnerRectTopRightPoint();
    auto p21 = getInnerRectBottomLeftPoint();
    auto p22 = getInnerRectBottomRightPoint();

    painter.drawLine(p11,p21);
    painter.drawLine(p11,p12);
    painter.drawLine(p12,p22);
    painter.drawLine(p21,p22);

    // 绘制内部线
    auto tops = getInnerVerticalLineTopPoints();
    auto bottoms = getInnerVerticalLineBottomPoints();
    auto lefts = getInnerHorizonalLineLeftPoints();
    auto rights = getInnerHorizonalLineRightPoints();

    Q_ASSERT(tops.count() == bottoms.count());
    Q_ASSERT(lefts.count() == rights.count());
    for(int i = 0; i < tops.count(); i++) {
        painter.drawLine(tops.at(i),bottoms.at(i));
    }
    for(int i = 0; i < lefts.count(); i++) {
        painter.drawLine(lefts.at(i),rights.at(i));
    }

    // 高亮鼠标区域
    auto rects = getInnerRects();
    LOG<<"rects.size = "<<rects.size()<<" mouse = "<<mMousePoint;
    if (mMousePoint != QPoint(-1,-1)) // 尚未点击
        painter.fillRect(rects[mMousePoint.x()][mMousePoint.y()],mMouseClickColor);
}

double PreviewPhotoCanvas::getInnerRectWidth() const
{
    return 2 * getExternalCircleRadius() / mrows *1.0;
}

double PreviewPhotoCanvas::getInnerRectHeight() const
{
    return 2 * getExternalCircleRadius() / mcols *1.0;
}

QPointFVector PreviewPhotoCanvas::getInnerHorizonalLineLeftPoints() const
{// 正方形左侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
    auto topleft = getInnerRectTopLeftPoint();
    for (int i = 1; i <= mcols-1; ++i) // i = 1开始是不包含topleft
        vec.append(topleft+ QPoint(0,i*offset)); // x不变,y增加
    return vec;
}

QPointFVector PreviewPhotoCanvas::getInnerHorizonalLineRightPoints() const
{// 正方形右侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
    auto topright = getInnerRectTopRightPoint();
    for (int i = 1; i <= mcols-1; ++i) // i = 1开始是不包含topright
        vec.append(topright+ QPoint(0,i*offset)); // x不变,y增加
    return vec;
}

QPointFVector PreviewPhotoCanvas::getInnerVerticalLineTopPoints() const
{// 正方形顶侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectWidth();
    auto topleft= getInnerRectTopLeftPoint();
    for (int i = 1; i <= mrows-1; ++i)
        vec.append(topleft+ QPoint(i*offset,0));
//    LOG<<"topleft = "<<topleft<<" offset = "<<offset<<" mrows = "<<mrows
//    <<" radius = "<<getExternalCircleRadius();
    return vec;
}

QPointFVector PreviewPhotoCanvas::getInnerVerticalLineBottomPoints() const
{// 正方形底侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectWidth();
    auto bottomleft= getInnerRectBottomLeftPoint();
    for (int i = 1; i <= mrows-1; ++i)
        vec.append(bottomleft+ QPoint(i*offset,0));
    return vec;
}

QPointF PreviewPhotoCanvas::getInnerRectTopLeftPoint() const
{
    return QPointF(width()/2.0-getExternalCircleRadius(),0);
}

QPointF PreviewPhotoCanvas::getInnerRectTopRightPoint() const
{
    return QPointF(width()/2.0+getExternalCircleRadius(),0);
}

QPointF PreviewPhotoCanvas::getInnerRectBottomLeftPoint() const
{
    return QPointF(width()/2.0-getExternalCircleRadius(),height());
}

QPointF PreviewPhotoCanvas::getInnerRectBottomRightPoint() const
{
    return QPointF(width()/2.0+getExternalCircleRadius(),height());
}

void PreviewPhotoCanvas::drawExternalCircle(QPainter &painter)
{
    auto radius = getExternalCircleRadius();
    painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
}

double PreviewPhotoCanvas::getExternalCircleRadius() const
{ // 圆半径
    return width()>=height()?height()/2.0:width()/2.0;
}

double PreviewPhotoCanvas::getExternalCircleRectSize() const
{ // 圆内接正方形尺寸
    return getExternalCircleRadius()* sqrt(2);
}

QPoint PreviewPhotoCanvas::getExternalCircleRectTopLeftPoint() const
{ // 窗口内有个圆,圆的内部有个内接正方形,正方形的左上角顶点 (w/2-r*sqrt(2)/2,r-r*sqrt(2)/2)
    auto radius = getExternalCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标为窗口的一半减去半径的45°对边
    auto y = radius - radius * sqrt(2)/2.0; // y坐标为半径减去半径的45°对边

    return QPoint(x,y);
}

QPoint PreviewPhotoCanvas::getExternalCircleRectTopRightPoint() const
{ // 窗口内有个圆,圆的内部有个内接正方形,正方形的右上角顶点 (w/2+r*sqrt(2)/2,r-r*sqrt(2)/2)
    auto radius = getExternalCircleRadius();
    auto x = width()/2.0 + radius * sqrt(2)/2.0; // x坐标为窗口的一半加上半径的45°对边
    auto y = radius - radius * sqrt(2)/2.0; // y坐标和左上角相同

    return QPoint(x,y);
}

QPoint PreviewPhotoCanvas::getExternalCircleRectBottomLeftPoint() const
{// 窗口内有个圆,圆的内部有个内接正方形,正方形的左下角顶点 (w/2-r*sqrt(2)/2,r+r*sqrt(2)/2)
    auto radius = getExternalCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标和左上角相同
    auto y = radius + radius * sqrt(2)/2.0; // y坐标为半径加上半径的45°对边

    return QPoint(x,y);
}

QPoint PreviewPhotoCanvas::getExternalCircleRectBottomRightPoint() const
{// 窗口内有个圆,圆的内部有个内接正方形,正方形的右下角顶点 (w/2+r*sqrt(2)/2,r+r*sqrt(2)/2)
    auto radius = getExternalCircleRadius();
    auto x = width()/2.0 - radius * sqrt(2)/2.0; // x坐标和右上角相同
    auto y = radius + radius * sqrt(2)/2.0; // y坐标和左下角相同

    return QPoint(x,y);
}

void PreviewPhotoCanvas::drawExternalCircleRect(QPainter &painter) {
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

QRectVector PreviewPhotoCanvas::getExternalCircleRects() const
{ // 获取圆内的所有正方形区域
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

void PreviewPhotoCanvas::setStrategy(PreviewPhotoCanvas::DrawStrategy s)
{
    strategy = s;
    mMousePoint = {-1,-1};
    update();
}

void PreviewPhotoCanvas::setStrategy(PreviewPhotoCanvas::DrawStrategy s, int rows, int cols)
{ // 使用绘制视野圆策略使用的方法
    strategy = s;
    mrows = rows;
    mcols = cols;
    mMousePoint = {-1,-1}; // 必须更新,否则上次的鼠标点还在会导致切换物镜或者brand出现越界
    update();
}

void PreviewPhotoCanvas::setExternalCircleRectSize(int size)
{
    mExternalCircleRectSize = size;
}