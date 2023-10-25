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
    QPoint mMousePoint = QPoint(-1,-1);
    QColor mMouseClickColor = Qt::green;
private:
    int mrows = 0;
    int mcols = 0;
    // 绘制内接圆用到的变量
    void drawInnerLine(QPainter&painter);
    double getInnerRectWidth() const; // 小矩形区域的宽度
    double getInnerRectHeight() const; // 小矩形区域的高度
    QPointF getInnerRectTopLeftPoint() const; // 外接正方形左上角顶点
    QPointF getInnerRectTopRightPoint() const;// 外接正方形右上角顶点
    QPointF getInnerRectBottomLeftPoint() const;// 外接正方形左下角顶点
    QPointF getInnerRectBottomRightPoint() const;// 外接正方形右下角顶点
    QPointFVector getInnerVerticalLineTopPoints() const ; // 正方形顶侧的等分点
    QPointFVector getInnerVerticalLineBottomPoints() const ;// 正方形底侧的等分点
    QPointFVector getInnerHorizonalLineLeftPoints() const ; // 正方形左侧的等分点
    QPointFVector getInnerHorizonalLineRightPoints() const ;// 正方形右侧的等分点
    QRectF2DVector getInnerRects() const; // 获取线分割出来的所有小矩形区域
    // 绘制外接圆用到的变量
    int mExternalCircleRectSize = 0;
    QRectVector getExternalCircleRects() const; //获取圆内的所有正方形区域
    double getExternalCircleRadius() const; // 圆半径
    double getExternalCircleRectSize() const; // 圆内接正方形尺寸
    QPoint getExternalCircleRectTopLeftPoint() const; // 圆内接正方形左上角顶点
    QPoint getExternalCircleRectBottomLeftPoint() const;// 圆内接正方形右上角顶点
    QPoint getExternalCircleRectTopRightPoint() const;// 圆内接正方形左下角顶点
    QPoint getExternalCircleRectBottomRightPoint() const;// 圆内接正方形右下角顶点
    void drawExternalCircle(QPainter&painter);
    void drawExternalCircleRect(QPainter&painter);
signals:
    void mouseClicked(const QPoint& point);
};

#endif //EOSI_PREVIEWPHOTOCANVAS_H
