/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 16:50:16
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 16:50:56
 * @FilePath: \EOS\interface\src\preview\slidepattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "slidepattern.h"

void SlidePattern::onSaveViewAct()
{
    if (!mDrapRectF.isEmpty()) {
        mSaveRectF = mDrapRectF;
        viewPoints();
    }
    update();
    emit rectUpdated(mSaveRectF);
    emit normRectUpdated(norm(mSaveRectF));
}

void SlidePattern::onRemoveViewAct()
{
    if (!mDrapRectF.isEmpty()) {
        if (mDrapRectF.intersects(mSaveRectF))
            mSaveRectF = QRectF();
    }
    update();
    emit rectUpdated(mSaveRectF);
    emit normRectUpdated(norm(mSaveRectF));
}

void SlidePattern::updateRect(const QRectF &rect)
{
    mSaveRectF = rect;
    update();
}

void SlidePattern::updateSize(int viewsize)
{
    mSize = viewsize;
    update();
}

bool SlidePattern::haveSlide() const
{
    return !mSaveRectF.isEmpty();
}

QPointFVector SlidePattern::viewPoints(bool overlap_) const
{ // 依据视野尺寸和保存的视野来得到电机坐标
    QPointFVector points;
    if (mSaveRectF.isEmpty()) return points;

    auto ref_w = qCeil(getReferenceWidth());
    auto ref_h = qCeil(getReferenceHeight());
    auto ref = getReferencePoint();

    // 1. 将要扫描的矩形取整
    auto x0 = qCeil(mSaveRectF.topLeft().x());// 电机坐标起点
    auto y0 = qCeil(mSaveRectF.topLeft().y());
    auto w = qCeil(mSaveRectF.width());
    auto h = qCeil(mSaveRectF.height());

    auto step_x = qCeil(ref_w / mSize); // 遍历的整数步进
    auto step_y = qCeil(ref_h / mSize);

    // 2. 得到离散的电机坐标并归一化
    for(int x = x0; x < x0 + w; x += step_x) {
        for(int y = y0; y < y0 + h; y += step_y) {
            //auto rect = QRectF(x,y,step_x,step_y);
            auto center_x = x+step_x/2.0;
            auto center_y = y+step_y/2.0;
            if (isValidPoint(QPointF(center_x,center_y)))
                points.append(QPointF((center_x - ref.x())/ref_w,(center_y - ref.y())/ref_h));
        }
    }

    // 3. 电机坐标重叠一定比例
    if (overlap_)
        points = overlap(points,overlapRate);
    //LOG<<points.count();
    return points;
}

void SlidePattern::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(DefaultPainterPenWidth);
    painter.setPen(pen);

    painter.drawRect(0,0,width(),height());
    painter.drawRect(getValidRect());
    painter.fillRect(mSaveRectF,Qt::green);

    if (mSize > 0) {
        auto ref_h = getReferenceHeight();
        auto ref_w = getReferenceWidth();
        auto topleft = getReferencePoint();
        auto bottomleft = topleft + QPointF(0.0,ref_h);
        auto topright = topleft + QPointF(ref_w,0.0);

        auto hor_offset = ref_w / mSize;
        pen.setWidth(DefaultPainterPenWidth/2);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        for (int i = 1; i <= mSize-1; ++i) { // 绘制垂直线,2个y坐标固定
            auto top = topleft + QPointF(i*hor_offset,0);
            auto bottom = bottomleft + QPointF(i*hor_offset,0);
            painter.drawLine(top,bottom);
        }
        auto ver_offset = ref_h / mSize;
        for (int i = 1; i <= mSize-1; ++i){// 绘制水平线,2个x坐标固定
            auto left = topleft + QPointF(0,ver_offset*i);
            auto right = topright + QPointF(0,ver_offset*i);
            painter.drawLine(left,right);
        }

        // 方便比对电机坐标是否正确
        auto x0 = qCeil(mSaveRectF.topLeft().x());// 电机坐标起点
        auto y0 = qCeil(mSaveRectF.topLeft().y());
        auto w = qCeil(mSaveRectF.width());
        auto h = qCeil(mSaveRectF.height());

        auto step_x = qCeil(ref_w / mSize); // 遍历的整数步进
        auto step_y = qCeil(ref_h / mSize);

        for(int x = x0; x < x0 + w; x += step_x) {
            for(int y = y0; y < y0 + h; y += step_y) {
                auto rect = QRectF(x,y,step_x,step_y);
                auto center_x = x+step_x/2.0;
                auto center_y = y+step_y/2.0;

                pen.setWidth(DefaultPainterPenWidth);
                pen.setStyle(Qt::DashLine);
                pen.setColor(PurpleEA3FF7);
                painter.setPen(pen);
                painter.drawRect(rect);

//                pen.setWidth(DefaultPainterPenWidth*2);
//                painter.setPen(pen);
//                if (isValidPoint(QPointF(center_x,center_y)))
//                    painter.drawPoint(center_x,center_y); // 无效的点不会被绘制和选中
            }
        }
        // 这里画点正确证明viewPoints计算没有问题
        auto viewpts = viewPoints(false);
        for(auto pt: viewpts) {
            pen.setWidth(DefaultPainterPenWidth*2);
            painter.setPen(pen);
            painter.drawPoint(pt.x()*ref_w+topleft.x(),pt.y()*ref_h+topleft.y());
        }
    }

    pen.setWidth(DefaultPainterPenWidth);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.drawRect(mDrapRectF);
    pen.setColor(Qt::black);
    painter.setPen(pen);
}

void SlidePattern::mousePressEvent(QMouseEvent *event)
{
    mMousePos = event->pos();
    if (!isValidPoint(mMousePos)) {
        mMousePos = {-1.0,-1.0};
    }

    if (event->button() == Qt::LeftButton) {
        mDrapRectF = QRectF();
    } else if (event->button() == Qt::RightButton) {
        if (mMousePos == QPointF(-1.0,-1.0)) {
            saveviewact->setEnabled(false);
            removeviewact->setEnabled(false);
        } else {
            saveviewact->setEnabled(true);
            removeviewact->setEnabled(true);
        }
    }
    if (mMousePos != QPointF(-1.0,-1.0)) {
        auto pos = norm(mMousePos);
        LOG<<"slide preview event point: "<<pos;
        emit previewEvent(pos);
    }
    update();
    event->accept();
}

void SlidePattern::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        auto end = event->pos(); // 鼠标停下的点
        auto rect = QRectF(mMousePos,end);
        if (!isValidRect(rect))
            return;
        mDrapRectF = rect;
    }
    update();
    event->accept();
}

void SlidePattern::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked();
}

SlidePattern::SlidePattern(QWidget*parent): QWidget(parent)
{
    saveviewact = new QAction(tr(SaveViewActTitle));
    removeviewact = new QAction(tr(RemoveViewActTitle));
    addAction(saveviewact);
    addAction(removeviewact);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(saveviewact,&QAction::triggered,this,&SlidePattern::onSaveViewAct);
    connect(removeviewact,&QAction::triggered,this,&SlidePattern::onRemoveViewAct);
}

bool SlidePattern::isValidPoint(const QPointF &point) const
{
    return getValidRect().contains(point);
}

bool SlidePattern::isValidRect(const QRectF &rect) const
{
    return getValidRect().contains(rect);
}

QRectF SlidePattern::getValidRect() const
{
    return QRectF(width()*gapRate,height()*gapRate,
                  width()*(1.0-2*gapRate),height()*(1.0-2*gapRate));
}

double SlidePattern::getReferenceHeight() const
{
    return height() * (1.0-2*gapRate);
}

double SlidePattern::getReferenceWidth() const
{
    return width() * (1.0-2*gapRate);
}

QPointF SlidePattern::getReferencePoint() const
{
    return QPointF(width()*gapRate,height()*gapRate);
}

QPointF SlidePattern::norm(const QPointF &point) const
{
    auto pos = point - getReferencePoint();
    pos = QPointF(pos.x()/getReferenceWidth(),pos.y()/getReferenceHeight());
    return pos;
}

QRectF SlidePattern::norm(const QRectF &rect) const
{
    auto topleft = rect.topLeft();
    auto size = rect.size();
    auto ref = getReferencePoint();
    auto ref_w = getReferenceWidth();
    auto ref_h = getReferenceHeight();

    auto x = (topleft.x() - ref.x()) / ref_w;
    auto y = (topleft.y()-ref.y()) / ref_h;
    auto w = size.width() / ref_w;
    auto h = size.height() / ref_h;

    return QRectF(x,y,w,h);
}

QPointFVector SlidePattern::overlap(const QPointFVector &points, double rate) const
{
    if (points.isEmpty())  // 删点时可能为空,下边索引0就越界了
        return points;

    QPointFVector pts;
    pts.append(points[0]);
    auto x0 = points[0].x();
    auto y0 = points[0].y();

    for(int i = 1; i < points.count(); ++i) {
        auto x = points[i].x();
        auto y = points[i].y();

        if (x >= x0) x *= (1.0-rate); // 在基点右侧,x应该减少才是靠近
        else x *= (1.0+rate); // 基点左侧,x增加才是靠近
        if (y < y0) y *= (1.0+rate); // 在基点上边,y应该增加是靠近
        else y *= (1.0-rate);

        pts.append(QPointF(x,y));
    }
    return pts;
}