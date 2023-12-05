#ifndef EOSI_VIEWPATTERN_H
#define EOSI_VIEWPATTERN_H

#include "window.h"

class INTERFACE_IMEXPORT ViewPattern: public QWidget
{
    Q_OBJECT
public:
    enum ViewSelectMode {PointMode,RectMode};
    explicit ViewPattern(QWidget*parent= nullptr);
    void setViewInfo(const ViewInfo& info);
    ViewInfo viewInfo() const;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void clearViewWindowCache(const QPoint &holePoint);
    void clearAllViewWindowCache(int viewSize,bool toggleObjective = true);
    void updateViewWindowCache(QCPoint holePoint, QCPointFVector viewPoints,int viewSize);

    void setDisablePoint(QCPoint point, bool enable = true);
    void setDisablePoints(QCPointVector points, bool enable = true);
    void setDisablePoints(bool enable = true);
private:
    int mSize = 0;
    //QMap<int,QBool2DVector> mUiViewMask;
    QBool2DVector mUiViewMask;
    const int mUiViewMaskSize = DefaultUiMaskSize; // 用于绘图 50-100就可以了
    void initUiViewMask();
    void viewRectsMapToViewMask();
    QPointF mapFromPointF(const QPointF& point) const;
    QPointF mapToPointF(const QPointF& point) const;
    QRectF mapToSize(const QRectF& source,const QPointF&ref_point,int ref_w,int ref_h);
    QRectF mapFromSize(const QRectF& source,const QPointF&ref_point,int ref_w,int ref_h);
private:
    ViewSelectMode mSelectMode = PointMode;
    ViewInfo mViewInfo;
    QPointF mMousePos;
    // 区域模式
    ViewPointVector mViewMaskNormPoints;
    QMap<int,ViewPointVector> mUiViewMaskNormPoints;
    QMap<int,ViewPointVector> mTmpUiViewMaskNormPoints;
    QRectF mMouseRect;
    QRectF mDrapRectF;
    QMap<int,ViewRectFVector> mViewRects;
    QMap<int,ViewRectFVector> mTmpRects;
    // 点模式
    QMap<int,QPointFVector> mUiSelectedPoints;
    QMap<int,QPointFVector> mTmpUiSelectedPoints;
    QPointFVector mSelectedPoints;
private:
    QAction * saveviewact;
    QAction * removeviewact;
    QAction * applyholeact;
    QAction * applygroupact;
    QAction * applyallact;
    void onRemoveViewAct();
    void onSaveViewAct();
    void onApplyHoleAct();
    void onApplyGroupAct();
    void onApplyAllAct();
    int holeID() const;
    int holeID(const QPoint& holePoint) const;
    bool checkField() const;
private:
    double getCircleRadius() const; // 视野圆半径
    double getInnerRectWidth() const; // 小矩形区域的宽度
    double getInnerRectHeight() const; // 小矩形区域的高度
    QPointF getInnerRectTopLeftPoint() const; // 外接正方形左上角顶点
    QPointF getInnerRectTopRightPoint() const;// 外接正方形右上角顶点
    QPointF getInnerRectBottomLeftPoint() const;// 外接正方形左下角顶点
    QPointF getInnerRectBottomRightPoint() const;// 外接正方形右下角顶点
    QRectF getValidRect() const;
signals:
    void previewEvent(const QPointF& point);
    void applyHoleEvent(const QVariantMap&m);
    void applyGroupEvent(const QVariantMap&m);
    void applyAllEvent(const QVariantMap&m);
};
#endif //EOSI_VIEWPATTERN_H
