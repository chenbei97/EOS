/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:20
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:34
 * @FilePath: \EOS\interface\include\preview\photocanvas.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PREVIEWPHOTOCANVAS_H
#define EOSI_PREVIEWPHOTOCANVAS_H

#include "window.h"

class INTERFACE_IMEXPORT PreviewPhotoCanvas: public QWidget
{
    Q_OBJECT
public:
    enum DrawStrategy {NoStrategy,ExternalCircleRect,InnerCircleRect};
    void setStrategy(DrawStrategy s);
    void setStrategy(DrawStrategy s,int rows,int cols);

    void setExternalCircleRectSize(int size);

    explicit PreviewPhotoCanvas(QWidget*parent= nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    DrawStrategy strategy = NoStrategy;
private:
    int mrows = 0;
    int mcols = 0;
    // 绘制内接圆用到的变量
    void drawInnerLine(QPainter&painter);
    // 绘制外接圆用到的变量
    int mrectsize = 0;
    QRectVector getExternalCircleRects() const; //获取圆内的所有正方形区域
    int getExternalCircleRadius() const; // 圆半径
    int getExternalCircleRectSize() const; // 圆内接正方形尺寸
    QPoint getExternalCircleRectTopLeftPoint() const; // 圆内接正方形左上角顶点
    QPoint getExternalCircleRectBottomLeftPoint() const;// 圆内接正方形右上角顶点
    QPoint getExternalCircleRectTopRightPoint() const;// 圆内接正方形左下角顶点
    QPoint getExternalCircleRectBottomRightPoint() const;// 圆内接正方形右下角顶点
    void drawExternalCircle(QPainter&painter);
    void drawExternalCircleRect(QPainter&painter);
};

#endif //EOSI_PREVIEWPHOTOCANVAS_H
