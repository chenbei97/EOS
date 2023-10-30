/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 09:28:54
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 09:29:59
 * @FilePath: \EOS\component\include\widgets\trianglemove.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_TRIANGLEMOVE_H
#define EOSI_TRIANGLEMOVE_H

#include "widgets.h"
#include "alias.h"

class COMPONENT_IMEXPORT TriangleMove: public QWidget
{
    Q_OBJECT
public:
    explicit TriangleMove(QWidget*parent= nullptr);
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void setTriangleGap(double gap);
    void setTriangleLength(double len);
private:
    QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
    double trianglen = TriangleLength;
    double trianggap = TriangleGap;
private:
    QPolygonF getLeftTrianglePoints() const;
    QPolygonF getRightTrianglePoints() const;
    QPolygonF getTopTrianglePoints() const;
    QPolygonF getBottomTrianglePoints() const;
signals:
    void leftTriangleClicked();
    void rightTriangleClicked();
    void topTriangleClicked();
    void bottomTriangleClicked();
};

#endif //EOSI_TRIANGLEMOVE_H
