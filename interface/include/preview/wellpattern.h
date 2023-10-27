/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:51:09
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-27 08:53:54
 * @FilePath: \EOS\interface\include\preview\wellpattern.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_WELLPATTERN_H
#define EOSI_WELLPATTERN_H

#include "window.h"
// 结合previewpattern的预览功能和protocolpattern分组功能
class INTERFACE_IMEXPORT WellPattern: public Pattern
{
    Q_OBJECT
public:
    explicit WellPattern(int rows,int cols,QWidget*parent= nullptr);
    void setPatternSize(int rows,int cols) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void setGroup(QCVariantMap m);
private:
    QRectF mDrapRect; // 鼠标拖动生成的矩形
    QBool2DVector mDrapPoints; // 拖拽矩形内选中的点赋值true
    QHoleInfo2DVector mHoleInfo; // 每个孔的所有信息用结构体封装
    QAction * groupact;
    QAction * viewact;
private:
    void initDrapPoints(); // 拖拽结束后清除这些点
    void initHoleInfo(); // 初始化所有孔内的包含信息
    int drapPointCount() const; // 计算拖拽区域包含的点个数
    void onSetGroupAct();
    void onSetViewAct();
signals:
    void drapEvent(const QVariantMap& m);
    void viewEvent(const QPoint& point);
};
#endif //EOSI_WELLPATTERN_H
