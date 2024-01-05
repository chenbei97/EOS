/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-29 14:09:27
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-29 14:17:51
 * @FilePath: \EOS\component\include\widgets\graphicsview.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_GRAPHICSVIEW_H
#define EOS_GRAPHICSVIEW_H

#include "widgets.h"
#include "qgl.h"

class COMPONENT_IMEXPORT GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QGraphicsScene*scene,QWidget*parent= nullptr);
    explicit GraphicsView(QWidget*parent= nullptr);
    void paintEvent(QPaintEvent*event) override;
private:
    void initAttributes();
};

#endif //EOS_GRAPHICSVIEW_H
