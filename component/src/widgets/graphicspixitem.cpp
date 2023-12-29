/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-29 14:26:53
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-29 15:15:31
 * @FilePath: \EOS\component\src\widgets\graphicspixitem.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "graphicspixitem.h"

GraphicsPixmapItem::GraphicsPixmapItem(const QPixmap& pixmap,QGraphicsItem *parent)
:QGraphicsPixmapItem(pixmap,parent)
{
    initAttributes();
}

GraphicsPixmapItem::GraphicsPixmapItem(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    initAttributes();
}

void GraphicsPixmapItem::setImage(const QImage &img)
{
    auto image = img.scaled(scene()->sceneRect().width(),scene()->sceneRect().height(),Qt::IgnoreAspectRatio,Qt::FastTransformation);
    setPixmap(QPixmap::fromImage(image));
}

void GraphicsPixmapItem::scaling(qreal rate)
{
    setTransformOriginPoint(boundingRect().center()); // 用自身的中心
    //setTransformOriginPoint(mapFromScene(scene()->sceneRect().center()));//用这个缩小到头再放大图形会瞎跳,飘

    if (rate >= 1.0) { // 说明是放大
        setScale(rate*scale());
    } else {
        if (scale()>0.15) { // 0.1不行会出现一些问题,0.12差不多是极限,考虑裕度0.15合适
            setScale(rate*scale()); // 对于pix不允许无限小,比例小于0.15就不设置了
        }
    }
}

void GraphicsPixmapItem::rotate(qreal angle)
{
//    if (scene() != nullptr) {
//        LOG<<boundingRect().center()<<scene()->sceneRect().center()<<mapFromScene(scene()->sceneRect().center());
//    }
    setTransformOriginPoint(boundingRect().center()); // 旋转应该以中心为锚点 2个都可以
    //setTransformOriginPoint(mapFromScene(scene()->sceneRect().center())); // 场景中心也可以但是必须要先转换坐标
    auto rot = rotation();
    rot += 90.0;
    if (rot == 360.0)
        rot = 0.0;
    setRotation(rot);
}

void GraphicsPixmapItem::reset()
{
    setTransformOriginPoint(boundingRect().center());
    setScale(1.0);
    setRotation(0);
    if (scene()) {
        setPos(scene()->sceneRect().topLeft());
    }
}

void GraphicsPixmapItem::initAttributes()
{
    setTransformOriginPoint(boundingRect().center()); // 用自身的中心
    //setTransformOriginPoint(mapFromScene(scene()->sceneRect().center()));//旋转可以,缩放用这个会飘
    setFlag(QGraphicsItem::ItemIsMovable);
    setCacheMode(DeviceCoordinateCache);
    setShapeMode(BoundingRectShape);
    setTransformationMode(Qt::SmoothTransformation);
    setAcceptTouchEvents(true);
    setToolTip("this is an image item");
}