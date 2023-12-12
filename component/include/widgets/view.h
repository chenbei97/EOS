/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 09:14:23
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-07 13:36:19
 * @FilePath: \EOS\component\include\widgets\view.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_VIEW_H
#define EOS_VIEW_H

#include "widgets.h"

class COMPONENT_IMEXPORT View: public QWidget
{
    Q_OBJECT
public:
    enum ViewSelectMode {PointMode,RectMode,WholeMode};
    explicit View(QWidget*parent= nullptr);

    virtual void setViewInfo(const ViewInfo& info);
    ViewInfo viewInfo() const;
    void setViewSize(int size);
    int viewSize() const;
    int holeID() const;
    int holeID(const QPoint& holePoint) const;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void setDisableRect(Qt::Alignment direction,double rate);
    void setDisableRect(double rate);
    void setDisableRect(const QMap<Qt::Alignment,double> &map);
    void clearDisableRect();
    QRectF getLeftDisableRect() const;
    QRectF getRightDisableRect() const;
    QRectF getTopDisableRect() const;
    QRectF getBottomDisableRect() const;
    void drawDisableLines(QPainter& painter,const QRectF& rect,const QColor& color,Qt::Orientation oriention);
protected:
    ViewSelectMode mSelectMode = PointMode;
    ViewInfo mViewInfo;
    int mSize = 0;
    double overlapRate = 0.1;
    QMap<Qt::Alignment,double> mDisableRectRates;
    QBool2DVector mDispersedMask;
    const int mDispersedMaskSize = DefaultDispersedMaskSize;
    QPointF mMousePos;
    QPointF mValidMousePos;
    QRectF mMouseRect;
    QRectF mDrapRectF;
    QAction * saveviewact;
    QAction * removeviewact;
    QAction * applyholeact;
    QAction * applygroupact;
    QAction * applyallact;
    virtual void onRemoveViewAct();
    virtual void onSaveViewAct();
    virtual void onApplyHoleAct();
    virtual void onApplyGroupAct();
    virtual void onApplyAllAct();
protected:
    QPointFVector overlap(const QPointFVector& points, double rate);
    void initDispersedMask();
    QPointF mapFromPointF(const QPointF& point) const;
    QPointF mapToPointF(const QPointF& point) const;
    QRectF mapToSize(const QRectF& source,const QPointF&ref_point,int ref_w,int ref_h) const;
    QRectF mapFromSize(const QRectF& source,const QPointF&ref_point,int ref_w,int ref_h) const;
    virtual bool checkField() const;
    double getCircleRadius() const; // 视野圆半径
    double getInnerRectWidth() const; // 小矩形区域的宽度
    double getInnerRectHeight() const; // 小矩形区域的高度
    QPointF getExternalRectTopLeftPoint() const; // 外接正方形左上角顶点
    QPointF getExternalRectTopRightPoint() const;// 外接正方形右上角顶点
    QPointF getExternalRectBottomLeftPoint() const;// 外接正方形左下角顶点
    QPointF getExternalRectBottomRightPoint() const;// 外接正方形右下角顶点
    QRectF getCircleInnerRect() const;
    QRectF getCircleExternalRect() const;
    bool isValidPoint(const QPointF& point) const;
    bool isValidRect(const QRectF& rect) const;
protected:
    bool isHighlight = false;
    double trianglen = ViewTriangleLength; // 三角的边长
    QColor highcolor  = Qt::green;
    QRectF getTriangleInnerRect() const;
    QPolygonF getLeftTrianglePoints() const;
    QPolygonF getRightTrianglePoints() const;
    QPolygonF getTopTrianglePoints() const;
    QPolygonF getBottomTrianglePoints() const;
signals:
    void previewEvent(const QPointF& point);
    void applyHoleEvent(const QVariantMap&m);
    void applyGroupEvent(const QVariantMap&m);
    void applyAllEvent(const QVariantMap&m);
    void leftTriangleClicked();
    void rightTriangleClicked();
    void topTriangleClicked();
    void bottomTriangleClicked();
    void triangleClicked(int option);
};

#endif //EOS_VIEW_H
