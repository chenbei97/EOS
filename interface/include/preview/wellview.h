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
    void toggleBrandObjective(int viewSize,bool toggleObjective = true);
    void adjustViewPoint(int option);
    void importViewInfoV1(QCPoint holePoint, QCPointFVector viewPoints,int viewSize);
    void importViewInfo(const QHoleInfoVector& vec);
    void setSelectMode(ViewSelectMode mode);
private:
    // 区域模式: 电机坐标!=UI离散坐标
    QMap<int,QPointFVector> mViewRectDispersedPoints; // UI离散坐标
    QMap<int,QPointFVector> mTmpRectDispersedPoints;
    QMap<int,QRectFVector> mViewRects; // UI离散区域=>UI离散坐标
    QMap<int,QRectFVector> mTmpRects;
    void dispersedViewRects();
    QPointFVector getViewPoints(int id,bool norm = true) const;
    // 点模式: 电机坐标=UI离散坐标
    QMap<int,QPointFVector> mViewPoints;
    QMap<int,QPointFVector> mTmpPoints;
    // 电机坐标: 2种模式共用
    QPointFVector mViewMachinePoints;
    //

private:
    virtual void onRemoveViewAct() override;
    virtual void onSaveViewAct() override;
    virtual void onApplyHoleAct() override;
    virtual void onApplyGroupAct() override;
    virtual void onApplyAllAct() override;
signals:
};
#endif //EOSI_VIEWPATTERN_H
