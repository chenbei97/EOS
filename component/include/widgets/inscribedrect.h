/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 13:28:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 13:28:29
 * @FilePath: \EOS\component\src\widgets\inscribedrect.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#ifndef EOSI_INSCRIBEDRECT_H
#define EOSI_INSCRIBEDRECT_H

#include "widgets.h"
#include "alias.h"

class COMPONENT_IMEXPORT InscribedRect : public QWidget
{
    Q_OBJECT
public:
    explicit InscribedRect(QWidget*parent= nullptr);
    explicit InscribedRect(int row,int col,QWidget*parent= nullptr);
    void setPatternSize(int row,int col);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void setInnerRectSize(int size);// 圆内接正方形小方格的尺寸
private:
    QPoint mMousePoint = QPoint(-1,-1);
    QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
    QColor mMouseClickColor = Qt::green;
    QRectF mDrapRect = QRectF(); // 鼠标拖动生成的矩形
    int mrows = 0;
    int mcols = 0;
    int mInnerRectSize = InscribedRectSize;

    double getCircleRadius() const;
    QRectFVector getInnerRectsVector() const; //获取圆内接正方形内的所有小正方形区域
    QRectF2DVector getInnerRects2DVector() const; //获取圆内接正方形内的所有小正方形区域
    double getInnerRectSize() const; // 圆内接正方形的尺寸
    QPointF getInnerRectTopLeftPoint() const; // 圆内接正方形左上角顶点
    QPointF getInnerRectBottomLeftPoint() const;// 圆内接正方形右上角顶点
    QPointF getInnerRectTopRightPoint() const;// 圆内接正方形左下角顶点
    QPointF getInnerRectBottomRightPoint() const;// 圆内接正方形右下角顶点
    void drawCircle(QPainter &painter); // 绘制圆
    void drawInnerRect(QPainter &painter); // 绘制内接正方形
    void fillInnerRect(QPainter&painter);//高亮圆内接的小矩形
signals:
    void mouseClicked(const QPoint& point);
};

#endif //EOSI_INSCRIBEDRECT_H
