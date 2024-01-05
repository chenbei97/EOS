/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 11:10:34
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-14 11:35:34
 * @FilePath: \EOS\component\src\widgets\photocanvastriangle.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "photocanvastriangle.h"

PhotoCanvasTriangle::PhotoCanvasTriangle(QWidget*parent): PhotoCanvas(parent)
{
    mLastPos = QPoint(-1,-1);
    mMousePoint = QPoint(-1,-1);
    highcolor  = Qt::green;
    highcolor.setAlpha(DefaultColorAlpha);
    trianglen = PhotoCanvasTriangleLength;
    trianggap = PhotoCanvasTriangleGap;

    rotateAct = new QAction(tr(RotateField));
    resetAct = new QAction(tr(ResetField));
    nomirroract = new QAction(tr(NoMirrorField));
    hormirroract = new QAction(tr(HorizontalMirrorField));
    vermirroract = new QAction(tr(VerticalMirrorField));
    allmirroract = new QAction(tr(AllMirrorField));
    auto sep = new QAction();
    sep->setSeparator(true);

    addAction(rotateAct);
    addAction(resetAct);
    addAction(sep);
    addAction(nomirroract);
    addAction(hormirroract);
    addAction(vermirroract);
    addAction(allmirroract);

    QActionGroup * actionGroup = new QActionGroup(this);
    actionGroup->addAction(nomirroract);
    actionGroup->addAction(hormirroract);
    actionGroup->addAction(vermirroract);
    actionGroup->addAction(allmirroract);
    actionGroup->setExclusive(true);
    nomirroract->setCheckable(true);
    hormirroract->setCheckable(true);
    vermirroract->setCheckable(true);
    allmirroract->setCheckable(true);
    nomirroract->setChecked(true);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(rotateAct,&QAction::triggered,this,&PhotoCanvasTriangle::rotate);
    connect(resetAct,&QAction::triggered,this,&PhotoCanvasTriangle::reset);
    connect(actionGroup,&QActionGroup::triggered,this,&PhotoCanvasTriangle::mirror);
    connect(CpuMemoryMonitorPointer,&CpuMemoryMonitor::monitorValue,this,&PhotoCanvasTriangle::updateCpuMemoryValue);
    CpuMemoryMonitorPointer->start(1000);
    installEventFilter(this);
}

void PhotoCanvasTriangle::reset()
{
    zoomRate = 1.0;
    rotateAngle = 0.0;
    mirrorType = MirrorType::NoMirror;
    nomirroract->setChecked(true);
    this->move(0,0);
    update();
}

void PhotoCanvasTriangle::rotate()
{
    rotateAngle += 90.0;
    if (rotateAngle == 360.0)
        rotateAngle = 0.0;
    update();
}

void PhotoCanvasTriangle::mirror(QAction *act)
{
    if (act == nomirroract) {
        mirrorType = MirrorType::NoMirror;
    } else if (act == hormirroract) {
        mirrorType = MirrorType::HorMirror;
    } else if (act == vermirroract) {
        mirrorType = MirrorType::VerMirror;
    } else if (act == allmirroract) {
        mirrorType = MirrorType::AllMirror;
    }
    update();
}

void PhotoCanvasTriangle::updateCpuMemoryValue(quint64 cpu, quint64 memory)
{
    cpu_percent = cpu;
    memory_percent = memory;
    update();
}

bool PhotoCanvasTriangle::eventFilter(QObject *watched, QEvent *event)
{
    auto type = event->type();
    if (type == QEvent::MouseMove) {
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        int dx = mouseEvent->pos().x() - mLastPos.x();
        int dy = mouseEvent->pos().y() - mLastPos.y();
        this->move(this->x() + dx, this->y() + dy);
        return true;
    } else if (type == QEvent::Wheel && mStrategy == SinglePixmap) {
        auto wheelEvent = static_cast<QWheelEvent*>(event);
        auto pixel = wheelEvent->angleDelta();
        if (pixel.y() > 0) {
            zoomRate = zoomRate * 1.05;
        } else {
            zoomRate = zoomRate * 0.95;
        }
        update();
        return true;
    }
    return PhotoCanvas::eventFilter(watched, event);
}

void PhotoCanvasTriangle::mousePressEvent(QMouseEvent *event)
{
    PhotoCanvas::mousePressEvent(event);
    mLastPos = event->pos();
    update();

    auto leftpoly = getLeftTrianglePoints();
    auto rightpoly = getRightTrianglePoints();
    auto toppoly = getTopTrianglePoints();
    auto bottompoly = getBottomTrianglePoints();

    if (leftpoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"left Triangle";
        emit leftTriangleClicked();
        emit triangleClicked(0);
    } else if (rightpoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"right Triangle";
        emit rightTriangleClicked();
        emit triangleClicked(2);
    } else if (toppoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"top Triangle";
        emit topTriangleClicked();
        emit triangleClicked(1);
    } else if (bottompoly.containsPoint(mLastPos,Qt::WindingFill)) {
        LOG <<"botttom Triangle";
        emit bottomTriangleClicked();
        emit triangleClicked(3);
    }

    event->accept();
}

void PhotoCanvasTriangle::mouseReleaseEvent(QMouseEvent *event)
{
    PhotoCanvas::mouseReleaseEvent(event);
    mLastPos = QPoint(-1,-1);
    update();
}

void PhotoCanvasTriangle::paintEvent(QPaintEvent *event)
{
    PhotoCanvas::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    auto font = painter.font();
    auto pointsize = font.pointSize(); // 像素比例
    double rate = 0.3;
    font.setPointSize(16);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(width()-height() * rate,100,QString("cpu: %1% memory: %2%").arg(cpu_percent).arg(memory_percent));
    font.setPixelSize(pointsize);
    painter.setFont(font);
    if (!isDraw) return;

    auto left = getLeftTrianglePoints();
    auto right = getRightTrianglePoints();
    auto top = getTopTrianglePoints();
    auto bottom = getBottomTrianglePoints();

    painter.drawPolygon(left);
    painter.drawPolygon(right);
    painter.drawPolygon(top);
    painter.drawPolygon(bottom);

    if (mLastPos == QPoint(-1,-1)) return;

    QPainterPath path;
    if (left.containsPoint(mLastPos,Qt::WindingFill))
        path.addPolygon(left);
    else if (right.containsPoint(mLastPos,Qt::WindingFill))
        path.addPolygon(right);
    else if (top.containsPoint(mLastPos,Qt::WindingFill))
        path.addPolygon(top);
    else if (bottom.containsPoint(mLastPos,Qt::WindingFill))
        path.addPolygon(bottom);

    painter.fillPath(path,highcolor);
}

void PhotoCanvasTriangle::drawTriangle(bool isDraw)
{
    this->isDraw = isDraw;
}

QPolygonF PhotoCanvasTriangle::getLeftTrianglePoints() const
{
    auto vertex = QPointF(0,height()/2);
    auto p_top = QPointF(trianglen*sqrt(3)/2,height()/2-trianglen/2);
    auto p_bottom = QPointF(trianglen*sqrt(3)/2,height()/2+trianglen/2);

    vertex += QPointF(trianggap,0);
    p_top += QPointF(trianggap,0);
    p_bottom += QPointF(trianggap,0);

    return QPolygonF(QPointFVector()<<vertex<<p_top<<p_bottom);
}

QPolygonF PhotoCanvasTriangle::getRightTrianglePoints() const
{
    auto vertex = QPointF(width(),height()/2);
    auto p_top = QPointF(width()-trianglen*sqrt(3)/2,height()/2-trianglen/2);
    auto p_bottom = QPointF(width()-trianglen*sqrt(3)/2,height()/2+trianglen/2);

    vertex += QPointF(-trianggap,0);
    p_top += QPointF(-trianggap,0);
    p_bottom += QPointF(-trianggap,0);

    return QPolygonF(QPointFVector()<<vertex<<p_top<<p_bottom);
}

QPolygonF PhotoCanvasTriangle::getTopTrianglePoints() const
{
    auto vertex = QPointF(width()/2,0);
    auto p_left = QPointF(width()/2-trianglen/2,trianglen*sqrt(3)/2);
    auto p_right = QPointF(width()/2+trianglen/2,trianglen*sqrt(3)/2);

    vertex += QPointF(0,trianggap);
    p_left += QPointF(0,trianggap);
    p_right += QPointF(0,trianggap);

    return QPolygonF(QPointFVector()<<vertex<<p_left<<p_right);
}

QPolygonF PhotoCanvasTriangle::getBottomTrianglePoints() const
{
    auto vertex = QPointF(width()/2,height());
    auto p_left = QPointF(width()/2-trianglen/2,height()-trianglen*sqrt(3)/2);
    auto p_right = QPointF(width()/2+trianglen/2,height()-trianglen*sqrt(3)/2);

    vertex += QPointF(0,-trianggap);
    p_left += QPointF(0,-trianggap);
    p_right += QPointF(0,-trianggap);

    return QPolygonF(QPointFVector()<<vertex<<p_left<<p_right);
}
