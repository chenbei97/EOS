/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-29 14:21:55
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-29 15:15:39
 * @FilePath: \EOS\component\include\widgets\graphicspixitem.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_GRAPHICSPIXITEM_H
#define EOS_GRAPHICSPIXITEM_H

#include "widgets.h"

class COMPONENT_IMEXPORT GraphicsPixmapItem: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit GraphicsPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    explicit GraphicsPixmapItem(QGraphicsItem *parent = nullptr);
    void rotate(qreal angle);
    void reset();
    void setImage(const QImage& img);
    void scaling(qreal rate);
private:
    void initAttributes();
};

#endif //EOS_GRAPHICSPIXITEM_H
