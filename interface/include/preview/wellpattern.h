/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:51:09
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 11:48:47
 * @FilePath: \EOS\interface\include\preview\wellpattern.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_WELLPATTERN_H
#define EOSI_WELLPATTERN_H

#include "window.h"

inline namespace V1 {
    class INTERFACE_IMEXPORT WellPattern: public Pattern
    {
        Q_OBJECT
    public:
        explicit WellPattern(int rows,int cols,QWidget*parent= nullptr);

        PreviewPatternInfo patternInfo() const;

        void setPatternSize(int rows,int cols) override;
        void initDrapPoints(); // 清除拖拽区域
        void initDisablePoints(); // 初始化置灰区域
        void setDisablePoints(bool enable = true);
        void setDisablePoint(QCPoint point,bool enable = true);
        void setDisablePoints(QCPointVector points, bool enable = true);

        int drapPointCount() const; // 计算拖拽区域包含的点个数
        void initHoleInfo(); // 初始化孔信息
        void clearAllHoleViewPoints();
        void updateHoleInfo(QCPoint point,QCString group,QCPointVector viewpoints,int viewsize);

        void paintEvent(QPaintEvent *event) override;
        QRectF2DVector getHoleRectsOnViewSize(QCPoint coordinate) const; // 拿到某个孔基于视野窗口尺寸来划分的所有小矩形区域
        QRectF2DVector getHoleRectsOnViewSize(int x,int y) const;

        void mouseMoveEvent(QMouseEvent *event) override;// 得到框选孔的区域
        void mousePressEvent(QMouseEvent *event) override;// 点左键要取消框选
        void mouseReleaseEvent(QMouseEvent *event) override;// 框选数量大于1不允许打开视野窗口,只能基于特定孔
        void mouseDoubleClickEvent(QMouseEvent*event) override;// 双击也能打开视野窗口

        int numberOfViews() const; // 计算已选择孔的所有视野数 用于计算占用空间
        int numberOfViews(const QString& group); // 这个组的视野数
        int numberOfViews(const QPoint& holepoint); // 这个孔的视野数
        QSet<QString> getAllWellGroupNames() const;// 返回所有分过的组,不重复
        void updateHoleInfoByGroupInfo(QCVariantMap m);// 右击打开分组窗口的信息(color+name)拿去更新孔数据
        void updateHoleInfoByViewInfoApplyHole(QCVariantMap m); // 删点保存点的映射
        void updateHoleInfoByViewInfoApplyGroup(QCVariantMap m); // 应用到组-视野窗口的信息拿去更新数据(PreviewPhotoCanvas::onApplyGroupAct())
        void updateHoleInfoByViewInfoApplyAll(QCVariantMap m);

    protected:
        QRectF mDrapRect; // 鼠标拖动生成的矩形
        QBool2DVector mDrapPoints; // 拖拽矩形内选中的点赋值true
        QBool2DVector mDisablePoints; // 置灰区域,不可选的区域
        QHoleInfo2DVector mHoleInfo; // 每个孔的所有信息用结构体封装
    protected:
        QAction * setgroupact;
        QAction * openviewact;
        QAction * removeholeact;
        void onSetGroupAct();// 当前孔的所属组颜色和名称传递给分组窗口去更新ui信息,触发openSetGroupWindow信号
        void onOpenViewAct();//打开选择视野窗口的事件,需要传递当前孔的coordinate+group+color+grouppoints+allgroups,触发openViewWindow信号
        void onRemoveHoleAct();
        QPointVector getHoleGroupCoordinates(const QString& groupName) const;// 获取组内的所有孔坐标传递给视野窗口在应用到本组时可以更新其它孔的视野数据区信息
        QPoint2DVector getAllWellHoleCoordinates() const;
    signals:
        void openSetGroupWindow(const QVariantMap& m); // 打开分组窗口事件,分组动作触发传递当前孔的color+group
        void openViewWindow(const QVariantMap& m); // 打开视野窗口的信号
        void clearViewWindowCache(const QPoint& holepoint); // 删孔需要清除视野窗口对应的缓存信息
    };
}
#endif //EOSI_WELLPATTERN_H
