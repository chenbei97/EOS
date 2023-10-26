/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 08:26:32
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 08:48:41
 * @FilePath: \EOS\component\include\widgets\pattern.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PATTERN_H
#define EOSI_PATTERN_H

#include "widgets.h"
#include "alias.h"
#include "groupinfo.h"
#include "circle.h"

class COMPONENT_IMEXPORT Pattern : public QWidget
{
    Q_OBJECT
public:
    explicit Pattern(int rows, int cols,QWidget*parent= nullptr);
    virtual void setPatternSize(int rows,int cols);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QPoint currentMousePoint() const;
    void clearMousePoint();//清除鼠标选中的圆
//    QSize sizeHint() const override;
protected:
    QSize getChildSize() const; // 每个圆都在划分的小正方形区域内,计算正方形的长度
    QRectF2DVector getChildRects() const; // 拿到所有小正方形的区域
    QPointF2DVector getBorderPoints() const;// 按行从左到右拿到所有分隔线上的点,可用于画线
    QPointFVector getRowHeaderPoints(); // 获取行表头文字的点,用于绘制文字
    QPointFVector getColHeaderPoints();// 获取列表头文字的点,用于绘制文字
    QPointF2DVector getCenterPoints() const; // 获取小正方形区域的中心用于绘制圆用
    void drawLine(QPainter&painter); // 绘制线
    void drawText(QPainter&painter); // 行列表头的文字
    void drawHighlight(QPainter&painter); // 绘制鼠标选中的高亮和已选择区域的高亮

protected:
    int mrows = 0;
    int mcols = 0;
    const int mGap = 30; // 圆之间的间隔
    const int mSpace = 10; // 到窗口边框留出的距离
    QColor mInnerCircleColor =  Qt::darkGray; // 默认填充内圆的颜色
    QColor mMouseClickColor = Qt::green; // 鼠标点击填充内圆的颜色
    bool mMouseEvent = false; // 是否启用鼠标事件
    QPoint mMousePos = QPoint(-1,-1); // 鼠标点击的坐标,不是真实物理坐标
    QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
signals:
    void mouseClicked(const QPoint& point);
};

#endif //EOSI_PATTERN_H
