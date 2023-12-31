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

#define use_imagetransformthread 1
#ifdef use_imagetransformthread
#include "imagetransformthread.h"
#endif

class COMPONENT_IMEXPORT PhotoCanvas: public QWidget
{
    Q_OBJECT
public:
    enum DrawStrategy {NoStrategy,SinglePixmap,GridPixmap};
    explicit PhotoCanvas(QWidget*parent= nullptr);
    ~PhotoCanvas();
    void setStrategy(DrawStrategy s,const QVariantMap& m = QVariantMap());
    PhotoCanvas::DrawStrategy strategy() const;

    void setGridSize(int gridsize);
    void clearGridImage();
    void appendImage(const QImage&img,const QPointF& point);
    void updateRect(const QRectF& rect);
    bool hasBoundingRect() const;

    void setImage(const QImage&img);
    void setImage(const QImage&img, int duration);
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

    void enableTransformThread(bool e);
protected:
    QImage mimage; // 单图模式使用
    double zoomRate = 1.0;
    double rotateAngle = 0.0;
    MirrorType mirrorType = MirrorType::NoMirror;
    void upateImageByThread(const QImage& img);
protected: // 多图模式使用
    int mGridSize = 0;
    QPairImageVector mGridImageVector;
    QPointFVector mGridCenterPoints;
    QRectF mBoundingRect = QRectF();
    void drawGridImage(QPainter&painter);
    double getInnerRectWidth() const;
    double getInnerRectHeight() const;
protected:
    DrawStrategy mStrategy = NoStrategy;
    QVariantMap mStrategyInfo;
    QRectF mDrapRect; // 鼠标拖动生成的矩形
    QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
    QColor mMouseClickColor = Qt::green;
    QTimer timer;
    quint64 cpu_percent = 0;
    quint64 memory_percent = 0;
protected:
    QPoint mMousePoint = QPoint(-1,-1);
signals:
    void mouseClicked(const QPoint& point);
    void mouseClicked();
    void doubleMouseClicked();
};

#endif //EOSI_PHOTOCANVAS_H
