/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 08:22:30
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 08:25:26
 * @FilePath: \EOS\interface\include\protocol\protocolpattern.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PROTOCOLPATTERN_H
#define EOSI_PROTOCOLPATTERN_H

#include "window.h"
#include "pattern.h"

class INTERFACE_IMEXPORT ProtocolPattern: public Pattern
{
    Q_OBJECT
public:
    explicit ProtocolPattern(int rows, int cols,QWidget*parent= nullptr);
    void setPatternSize(int rows,int cols) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    QRectF mDrapRect; // 鼠标拖动生成的矩形
    QBool2DVector mDrapPoints; // 拖拽矩形内选中的点赋值true
    QHoleInfo2DVector mHoleInfo; // 每个孔的所有信息用结构体封装
private:
    void initDrapPoints();
    void initHoleInfo();
    void select(QCColor color);
};
#endif //EOSI_PROTOCOLPATTERN_H
