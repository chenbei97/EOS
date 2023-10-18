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
private:
    const int rectwidth = 200;
    const int rectheight = 100;
    const int gap = 2;
    const int pointsize = 20;
    QStringList texts;
    QVector<int> pixwidths;
    int pixheight;
    QFont font;

};
#endif //EOSI_NAVIGBAR_H
