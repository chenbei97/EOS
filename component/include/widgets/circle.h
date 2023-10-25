/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 16:54:15
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 17:15:04
 * @FilePath: \EOS\component\include\widgets\circle.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_CIRCLE_H
#define EOSI_CIRCLE_H

#include "widgets.h"

class COMPONENT_IMEXPORT Circle: public QWidget
{
    Q_OBJECT
public:
    explicit Circle(QWidget*parent= nullptr);
    void paintEvent(QPaintEvent*event) override;
    void mousePressEvent(QMouseEvent *event) override;
protected:
    QPoint mMousePos = QPoint(-1,-1);
    QPoint getCenter() const;
    int getRadius() const;
signals:
    void mouseClicked(const QPoint& point);
};

#endif //EOSI_CIRCLE_H
