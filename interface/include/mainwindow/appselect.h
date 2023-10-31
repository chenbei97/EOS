/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 12:59:15
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 12:59:46
 * @FilePath: \EOS\interface\include\window\appselect.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_APPSELECT_H
#define EOSI_APPSELECT_H

#include "window.h"

class INTERFACE_IMEXPORT AppSelect: public QWidget
{
    Q_OBJECT
public:
    explicit AppSelect(QWidget*parent= nullptr);
    void paintEvent(QPaintEvent*event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    QVector<int> pixelwidths;
    void drawRoundRectV1(QPainter& painter);
    void drawRoundRectV2(QPainter& painter);
    QRectVector getRects() const; // 获取所有按钮的小正方形区域
signals:
    void proliferationClicked();
    void scratchClicked();
    void invasionClicked();
    void transfectionClicked();
    void morphologyClicked();
    void organoidClicked();
    void pharmtoxiClicked();
    void customClicked();
    void appClicked(int index);
    void appClicked(const QString& app);
};

#endif //EOSI_APPSELECT_H
