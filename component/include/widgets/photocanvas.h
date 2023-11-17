/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:20
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:34
 * @FilePath: \EOS\interface\include\preview\photocanvas.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PHOTOCANVAS_H
#define EOSI_PHOTOCANVAS_H

#include "widgets.h"
#include "qtimer.h"

class COMPONENT_IMEXPORT PhotoCanvas: public QWidget
{
    Q_OBJECT
public:
    enum DrawStrategy {NoStrategy,SinglePixmap};
    explicit PhotoCanvas(QWidget*parent= nullptr);
    void setStrategy(DrawStrategy s,const QVariantMap& m = QVariantMap());
    void setData(const QVariantMap& m);
    void setImage(const QImage&img);
    void setPixmap(const QPixmap& pix);
    QPixmap pixmap() const;
    QImage image() const;
    void optimizePaint(int ms = 100);
    void stopOptimizePaint();
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;// 左键点击清除框选,计算鼠标点击的小矩形区域坐标
    void mouseMoveEvent(QMouseEvent *event) override;// 绘制拖拽框
    void mouseReleaseEvent(QMouseEvent *event) override;// 拖拽区域点个数为0才是预览事件
    void paintEvent(QPaintEvent *event) override;
protected:
    DrawStrategy strategy = NoStrategy;
    QVariantMap mStrategyInfo;
    QImage mimage;
    QRectF mDrapRect; // 鼠标拖动生成的矩形
    QPoint mMousePoint = QPoint(-1,-1);
    QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
    QColor mMouseClickColor = Qt::green;
    QTimer timer;
signals:
    void mouseClicked(const QPoint& point);
    void mouseClicked();
    void doubleMouseClicked();
};

#endif //EOSI_PHOTOCANVAS_H
