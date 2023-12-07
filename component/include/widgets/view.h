/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 09:14:23
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 09:15:46
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
    int holeID() const;
    int holeID(const QPoint& holePoint) const;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void setDisablePoint(Qt::Alignment direction,double rate);
    void setDisablePoints(QCPointVector points, bool enable = true);
    void setDisablePoints(bool enable = true);
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
    const int mDispersedMaskSize = DefaultUiMaskSize;
    QPointF mMousePos;
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
    QRectF mapToSize(const QRectF& source,const QPointF&ref_point,int ref_w,int ref_h);
    QRectF mapFromSize(const QRectF& source,const QPointF&ref_point,int ref_w,int ref_h);
    virtual bool checkField() const;
    double getCircleRadius() const; // 视野圆半径
    double getInnerRectWidth() const; // 小矩形区域的宽度
    double getInnerRectHeight() const; // 小矩形区域的高度
    QPointF getInnerRectTopLeftPoint() const; // 外接正方形左上角顶点
    QPointF getInnerRectTopRightPoint() const;// 外接正方形右上角顶点
    QPointF getInnerRectBottomLeftPoint() const;// 外接正方形左下角顶点
    QPointF getInnerRectBottomRightPoint() const;// 外接正方形右下角顶点
    QRectF getValidRect() const;
    QRectF getInnerRect() const;
    bool isValidRect(const QPointF& point) const;
    bool isValidRect(const QRectF& rect) const;
signals:
    void previewEvent(const QPointF& point);
    void applyHoleEvent(const QVariantMap&m);
    void applyGroupEvent(const QVariantMap&m);
    void applyAllEvent(const QVariantMap&m);
};

#endif //EOS_VIEW_H
