/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-08 15:15:03
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-08 15:16:37
 * @FilePath: \EOS\component\src\widgets\picturecanvas.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "singlepicturecanvas.h"

void SinglePictureCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(DefaultPainterPenWidth);
    painter.setPen(pen);
    if(!mimage.isNull()) {
//  painter.drawImage(QRectF(width()*(1.0-zoomRate)/2.0,height()*(1.0-zoomRate)/2.0,
//                            width()*zoomRate,height()*zoomRate),mimage);
        painter.translate(width()/2,height()/2); // 或者把画笔挪到中间再画
        painter.drawImage(QRectF(-width()*zoomRate/2,-height()*zoomRate/2,
                                 width()*zoomRate,height()*zoomRate),mimage);
        painter.translate(0,0); // 防止对继承的或者后面的产生影响
    }
}

bool SinglePictureCanvas::eventFilter(QObject *watched, QEvent *event)
{
    auto type = event->type();
    if (type == QEvent::MouseMove) {
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        int dx = mouseEvent->pos().x() - mLastPos.x();
        int dy = mouseEvent->pos().y() - mLastPos.y();
        this->move(this->x() + dx, this->y() + dy);
        return true;
    } else if (type == QEvent::Wheel) {
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
    return QWidget::eventFilter(watched, event);
}

void SinglePictureCanvas::mousePressEvent(QMouseEvent *event)
{ // 左键点击清除框选,计算鼠标点击的小矩形区域坐标
    mLastPos = event->pos();
    event->accept();
}

void SinglePictureCanvas::setImage(const QImage &img)
{
    mimage = img;
    update();
}

void SinglePictureCanvas::setPixmap(const QPixmap &pix)
{
    mimage = pix.toImage();
    update();
}

void SinglePictureCanvas::setImage(const QString &path)
{
    mimage.load(path);
    update();
}

void SinglePictureCanvas::setImage(const QImage &img, const QString &path)
{
    mimage = QImage(path);
    Q_UNUSED(img); // 加载原图否则不清楚
    update();
}

SinglePictureCanvas::SinglePictureCanvas(QWidget *parent) : QWidget(parent)
{

}

