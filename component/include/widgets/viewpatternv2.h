/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 09:49:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 09:55:26
 * @FilePath: \EOS\component\include\widgets\viewpatternv2.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_VIEWPATTERNV2_H
#define EOS_VIEWPATTERNV2_H

#include "widgets.h"
#include "view.h"
#include "utilities.h"

namespace obsoleteClass { // 过时,开发了新版本
    const int DefaultUiMaskSize = 100;
#define PointToIDCoefficient 1000 // kx1+y1=kx2+y2=> k=(y2-y1)/(x1-x2),k取什么值?找不到这样的2个坐标能等于k,由于wellsize最大384,实际上k>384就能保证id唯一了
    class COMPONENT_IMEXPORT ViewPatternV2 : public View {
    Q_OBJECT
    public:
        explicit ViewPatternV2(QWidget *parent = nullptr);

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void paintEvent(QPaintEvent *event) override;

        void clearViewWindowCache(const QPoint &holePoint);

        void clearAllViewWindowCache(int viewSize, bool toggleObjective = true);

        void updateViewWindowCache(QCPoint holePoint, QCPointFVector viewPoints, int viewSize);

    private:
        //QMap<int,QBool2DVector> mUiViewMask;
        QBool2DVector mUiViewMask;
        const int mUiViewMaskSize = DefaultUiMaskSize; // 用于绘图 50-100就可以了
        void initUiViewMask();

        void viewRectsMapToViewMask();

    private:
        // 区域模式
        ViewPointVector mViewMaskNormPoints;
        QMap<int, ViewPointVector> mUiViewMaskNormPoints;
        QMap<int, ViewPointVector> mTmpUiViewMaskNormPoints;
        QMap<int, ViewRectFVector> mViewRects;
        QMap<int, ViewRectFVector> mTmpRects;
        // 点模式
        QMap<int, QPointFVector> mUiSelectedPoints;
        QMap<int, QPointFVector> mTmpUiSelectedPoints;
        QPointFVector mSelectedPoints;
    private:
        virtual void onRemoveViewAct() override;

        virtual void onSaveViewAct() override;

        virtual void onApplyHoleAct() override;

        virtual void onApplyGroupAct() override;

        virtual void onApplyAllAct() override;

        int holeID() const;

        int holeID(const QPoint &holePoint) const;
    };
}
#endif //EOS_VIEWPATTERNV2_H
