/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:42:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 10:42:59
 * @FilePath: \EOS\interface\include\navigbar.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_NAVIGBAR_H
#define EOSI_NAVIGBAR_H

#include "window.h"

class INTERFACE_IMEXPORT NavigBar: public QWidget
{
    Q_OBJECT
public:
    explicit NavigBar(QWidget*parent= nullptr);
    void paintEvent(QPaintEvent*event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;
private:
    QVector<int> pixwidths;
    QRectVector getRects() const; // 获取所有文字部分的小正方形区域
signals:
    void mainClicked();
    void previewClicked();
    void protocolClicked();
    void dataClicked();
    void analysisClicked();
};
#endif //EOSI_NAVIGBAR_H
