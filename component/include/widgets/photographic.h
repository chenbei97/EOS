/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-27 17:25:53
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-27 17:29:56
 * @FilePath: \EOS\component\include\widgets\photographic.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_PHOTOGRAPHIC_H
#define EOS_PHOTOGRAPHIC_H

#include "widgets.h"
#include "graphicsview.h"
#include "graphicspixitem.h"

//#define use_transform_view 1
class COMPONENT_IMEXPORT PhotoGraphics: public QWidget
{
    Q_OBJECT
public:
    explicit PhotoGraphics(QWidget*parent= nullptr);
    void resizeEvent(QResizeEvent*event) override;
    void wheelEvent(QWheelEvent*event) override;
    void setImage(const QImage& img,int duration);
private:
    double sceneWidth = 1200.;
    double sceneHeight = 800.;
    double sceneRefX = -sceneWidth/2.;
    double sceneRefY = -sceneHeight/2.;
    const double zoomOutRate = 1.05;
    const double zoomInRate = 0.95;
    const double sceneMargin = 50.;
    GraphicsView * graphicsview;
    QGraphicsScene * graphicsscene;
    GraphicsPixmapItem * pix;
    QAction * rotateact;
    QAction * resetact;
    void rotate();
    void reset();
};

#endif //EOS_PHOTOGRAPHIC_H
