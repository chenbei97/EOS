/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 11:36:10
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-14 11:48:48
 * @FilePath: \EOS\component\include\widgets\labeltriangle.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_LABELTRIANGLE_H
#define EOS_MAIN_CORE_PY_LABELTRIANGLE_H

#include "widgets.h"
#include "label.h"

class COMPONENT_IMEXPORT LabelTriangle: public Label
{
    Q_OBJECT
public:
    explicit LabelTriangle(QWidget*parent= nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
    double trianglen = PhotoCanvasTriangleLength;
    double trianggap = PhotoCanvasTriangleGap;
    QColor highcolor  = Qt::green;
    double zoomRate = 1.0;
    bool drawTriangle = false;
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
    void triangleClicked(int option);

};
#endif //EOS_MAIN_CORE_PY_LABELTRIANGLE_H
