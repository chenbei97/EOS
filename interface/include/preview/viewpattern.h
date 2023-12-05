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
    // 区域模式: 电机坐标!=UI离散坐标
    QMap<int,QPointFVector> mViewRectDispersedPoints; // UI离散坐标
    QMap<int,QPointFVector> mTmpRectDispersedPoints;
    QMap<int,QRectFVector> mViewRects; // UI离散区域=>UI离散坐标
    QMap<int,QRectFVector> mTmpRects;
    QPointFVector mViewMachinePoints; // 电机坐标
    void dispersedViewRects();
    // 点模式: 电机坐标=UI离散坐标
    QMap<int,QPointFVector> mViewPoints;
    QMap<int,QPointFVector> mTmpPoints;
private:
    virtual void onRemoveViewAct() override;
    virtual void onSaveViewAct() override;
    virtual void onApplyHoleAct() override;
    virtual void onApplyGroupAct() override;
    virtual void onApplyAllAct() override;
};
#endif //EOSI_VIEWPATTERN_H
