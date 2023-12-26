/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-20 13:50:18
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-20 13:51:20
 * @FilePath: \EOS\component\include\widgets\sliderx.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SLIDERX_H
#define EOSI_SLIDERX_H

#include "widgets.h"

class COMPONENT_IMEXPORT Sliderx: public QSlider
{
    Q_OBJECT
public:
    explicit Sliderx(QWidget*parent= nullptr);
    explicit Sliderx(Qt::Orientation orientation,QWidget*parent= nullptr);
    void mousePressEvent(QMouseEvent*event) override;
    void mouseReleaseEvent(QMouseEvent*event) override;
    void wheelEvent(QWheelEvent*event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void setMouseEvent(bool enabled);
private:
    bool enableMouse = true;
};
#endif //EOSI_SLIDERX_H
