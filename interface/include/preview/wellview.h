#ifndef EOSI_VIEWPATTERN_H
#define EOSI_VIEWPATTERN_H

#include "window.h"

class INTERFACE_IMEXPORT WellView: public View
{
    Q_OBJECT
public:
    explicit WellView(QWidget*parent= nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void removeHole(const QPoint &holePoint);
#ifdef viewRowColUnEqual
    void toggleBrandObjective(const Dimension2D& dimension,bool toggleObjective = true);
#else
    void toggleBrandObjective(int viewSize,bool toggleObjective = true);
#endif
    void adjustViewPoint(int option);
    //void importViewInfoV1(QCPoint holePoint, QCPointFVector viewPoints,int viewSize);
    void importViewInfo(const QHoleInfoVector& vec,ViewMode mode);
    void setViewMode(ViewMode mode);
private:
    // 区域模式: 电机坐标!=UI离散坐标,但是等于从viewRect获取的坐标
    QMap<int,QPointFVector> mViewRectDispersedPoints; // UI离散坐标dispersedViewRects当中计算
    QMap<int,QPointFVector> mTmpRectDispersedPoints;// UI离散坐标dispersedViewRects当中计算
    QMap<int,QRectFVector> mViewRects; // UI离散区域=>UI离散坐标
    QMap<int,QRectFVector> mTmpRects;// UI离散区域=>UI离散坐标
    void dispersedViewRects();
    QPointFVector getMachinePointsFromViewRect(int id,bool norm = true) const;//从离散区域获取中心的重叠的坐标作为电机坐标
    // 点模式: 电机坐标=UI离散坐标
    QMap<int,QPointFVector> mViewPoints;
    QMap<int,QPointFVector> mTmpPoints;
    // 电机坐标: 2种模式共用
    QPointFVector mViewMachinePoints;
    ViewApply * viewApply;
private:
    virtual void onRemoveViewAct() override;
    virtual void onSaveViewAct() override;
    virtual void onApplyHoleAct() override;
    virtual void onApplyGroupAct() override;
    virtual void onApplyAllAct() override;
signals:
};
#endif //EOSI_VIEWPATTERN_H
