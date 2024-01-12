/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 08:37:21
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:52:09
 * @FilePath: \EOS\interface\include\data\datapattern.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_DATAPATTERN_H
#define EOS_DATAPATTERN_H

#include "window.h"

class INTERFACE_IMEXPORT DataPattern: public Pattern
{
    Q_OBJECT
public:
    explicit DataPattern(int rows,int cols,QWidget*parent= nullptr);
    void setPatternSize(int rows, int cols);
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void initHoleInfo(const PlateImageInfo& info);
private:
    DataPatternHoleInfo2DVector mHoleInfo; // 每个孔的所有信息用结构体封装
    QBool2DVector mDisableHoles; // 置灰区域,不可选的区域
    void initHoleInfo(); // 初始化孔信息
    void initDisableHoles();
signals:
    void holeClicked(const DataPatternHoleInfo & info);
};

#endif //EOS_DATAPATTERN_H
