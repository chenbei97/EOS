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
    enum DrawStrategy {NoStrategy};
    explicit PreviewPhotoCanvas(QWidget*parent= nullptr);
    void setStrategy(DrawStrategy s,const QVariantMap& m = QVariantMap());
    QVariantMap currentHoleInfo() const;
    void mousePressEvent(QMouseEvent *event) override;// 左键点击清除框选,计算鼠标点击的小矩形区域坐标
    void mouseMoveEvent(QMouseEvent *event) override;// 绘制拖拽框
    void mouseReleaseEvent(QMouseEvent *event) override;// 拖拽区域点个数为0才是预览事件
    void paintEvent(QPaintEvent *event) override;
private:
    DrawStrategy strategy = NoStrategy;
    QVariantMap mCurrentHoleInfo;
    int mrows = 0;
    int mcols = 0;
    QRectF mDrapRect; // 鼠标拖动生成的矩形
    QPoint mMousePoint = QPoint(-1,-1);
    QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
    QColor mMouseClickColor = Qt::green;
signals:
    void mouseClicked(const QPoint& point);
};

#endif //EOSI_PREVIEWPHOTOCANVAS_H
