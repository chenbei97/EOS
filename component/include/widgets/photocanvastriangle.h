/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 11:09:01
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-14 11:35:27
 * @FilePath: \EOS\component\include\widgets\photocanvastriangle.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_PHOTOCANVASTRIANGLE_H
#define EOS_MAIN_CORE_PY_PHOTOCANVASTRIANGLE_H

#include "photocanvas.h"

class COMPONENT_IMEXPORT PhotoCanvasTriangle: public PhotoCanvas
{
    Q_OBJECT
public:
    explicit PhotoCanvasTriangle(QWidget*parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent*event) override;
    void drawTriangle(bool isDraw = true);
private:
    QPolygonF getLeftTrianglePoints() const;
    QPolygonF getRightTrianglePoints() const;
    QPolygonF getTopTrianglePoints() const;
    QPolygonF getBottomTrianglePoints() const;
private:
    double trianglen = PhotoCanvasTriangleLength; // 三角的边长
    double trianggap = PhotoCanvasTriangleGap; // 三角形到边缘的距离
    QColor highcolor  = Qt::green;
    bool isDraw = true;
signals:
    void leftTriangleClicked();
    void rightTriangleClicked();
    void topTriangleClicked();
    void bottomTriangleClicked();
    void triangleClicked(int option);
};

#endif //EOS_MAIN_CORE_PY_PHOTOCANVASTRIANGLE_H
