#ifndef EOSI_VIEWPATTERN_H
#define EOSI_VIEWPATTERN_H

#include "window.h"

class INTERFACE_IMEXPORT ViewPattern: public View
{
    Q_OBJECT
public:
    explicit ViewPattern(QWidget*parent= nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void clearViewWindowCache(const QPoint &holePoint);
    void clearAllViewWindowCache(int viewSize,bool toggleObjective = true);
    void updateViewWindowCache(QCPoint holePoint, QCPointFVector viewPoints,int viewSize);
private:
    //QMap<int,QBool2DVector> mUiViewMask;
    QBool2DVector mUiViewMask;
    const int mUiViewMaskSize = DefaultUiMaskSize; // 用于绘图 50-100就可以了
    void initUiViewMask();
    void viewRectsMapToViewMask();
private:
    // 区域模式
    ViewPointVector mViewMaskNormPoints;
    QMap<int,ViewPointVector> mUiViewMaskNormPoints;
    QMap<int,ViewPointVector> mTmpUiViewMaskNormPoints;
    QMap<int,QRectFVector> mViewRects;
    QMap<int,QRectFVector> mTmpRects;
    // 点模式
    QMap<int,QPointFVector> mUiSelectedPoints;
    QMap<int,QPointFVector> mTmpUiSelectedPoints;
    QPointFVector mSelectedPoints;
private:
    virtual void onRemoveViewAct() override;
    virtual void onSaveViewAct() override;
    virtual void onApplyHoleAct() override;
    virtual void onApplyGroupAct() override;
    virtual void onApplyAllAct() override;
    int holeID() const;
    int holeID(const QPoint& holePoint) const;
};
#endif //EOSI_VIEWPATTERN_H
