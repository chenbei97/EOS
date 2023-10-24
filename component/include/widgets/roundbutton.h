/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 09:00:52
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 09:21:42
 * @FilePath: \EOS\component\include\widgets\roundbutton.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_ROUNDBUTTON_H
#define EOSI_ROUNDBUTTON_H

#include "widgets.h"
#include "pushbutton.h"

class COMPONENT_IMEXPORT RoundButton: public PushButton
{
    Q_OBJECT
public:
    enum PaintStrategy {NoStrategy,RightTriangle,LeftTriangle,TopTriangle,BottomTriangle};
    explicit RoundButton(QWidget*parent= nullptr);
    void setSize(int size);
    void setStrategy(PaintStrategy s);

    void paintEvent(QPaintEvent*event) override;
    QSize sizeHint() const override;
private:
    PaintStrategy strategy;

    void drawRightTriangle(QPainter&painter);
    void drawLeftTriangle(QPainter&painter);
    void drawTopTriangle(QPainter&painter);
    void drawBottomTriangle(QPainter&painter);
};

#endif //EOSI_ROUNDBUTTON_H
