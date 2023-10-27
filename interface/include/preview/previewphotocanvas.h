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

    explicit PreviewPhotoCanvas(QWidget*parent= nullptr);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //void showEvent(QShowEvent*event) override;

    void setStrategy(DrawStrategy s);
    void setStrategy(DrawStrategy s,int rows,int cols);
    void setCurrentHoleInfo(const QVariantMap & m); //设置当前孔的一些信息

    void setExternalCircleRectSize(int size);// 圆内接正方形 小方格的尺寸
private:
    DrawStrategy strategy = NoStrategy;
    QPoint mMousePoint = QPoint(-1,-1);
    QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
    QColor mMouseClickColor = Qt::green;
    QRectF mDrapRect; // 鼠标拖动生成的矩形
    QBool2DVector mDrapPoints; // 拖拽矩形内选中的点赋值true
    //QBool2DVector mSelectPoints; // 确实选中的点标记其他颜色
    QMap<int,QBool2DVector> mHoleSelectPoints;
    QMap<int,QBool2DVector> mTmpHoleSelectPoints; // 临时变量
    QAction * viewact;
    QAction * applygroupact;
    QAction * applyallact;
    int mrows = 0;
    int mcols = 0;
private:
    QVariantMap currentHoleInfo;
private:
    void initDrapPoints(); // 拖拽结束后清除这些点
    void initSelectPoints(); // 被选中的点
    void onSetViewAct();
    int drapPointCount() const;
private:
    void drawDrapRect(QPainter&painter);// 绘制矩形框
    void drawSelectRect(QPainter&painter); // 绘制被选中的区域
    void drawCircle(QPainter&painter); // 画大圆
    double getCircleRadius() const; // 圆半径
private:
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
private:
    // 绘制外接圆用到的变量
    int mExternalCircleRectSize = 0;
    QRectVector getExternalCircleRects() const; //获取圆内的所有正方形区域

    double getExternalCircleRectSize() const; // 圆内接正方形尺寸
    QPoint getExternalCircleRectTopLeftPoint() const; // 圆内接正方形左上角顶点
    QPoint getExternalCircleRectBottomLeftPoint() const;// 圆内接正方形右上角顶点
    QPoint getExternalCircleRectTopRightPoint() const;// 圆内接正方形左下角顶点
    QPoint getExternalCircleRectBottomRightPoint() const;// 圆内接正方形右下角顶点

    void drawExternalCircleRect(QPainter&painter);
signals:
    void mouseClicked(const QPoint& point);
    void previewEvent(const QPoint& point); // 单击非框选时是预览
    void drapEvent();// 框选才会
};

#endif //EOSI_PREVIEWPHOTOCANVAS_H
