/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-08 15:14:05
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-08 15:16:43
 * @FilePath: \EOS\component\include\widgets\picturecanvas.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */


#ifndef EOS_SINGLEPICTURECANVAS_H
#define EOS_SINGLEPICTURECANVAS_H

#include "widgets.h"

class COMPONENT_IMEXPORT SinglePictureCanvas: public QWidget
{
    Q_OBJECT
public:
    explicit SinglePictureCanvas(QWidget*parent= nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void setImage(const QImage&img);
    void setPixmap(const QPixmap& pix);
    void setImage(const QString& path);
private:
    QImage mimage; // 单图模式使用
    double zoomRate = 1.0;
    QPoint mLastPos = QPoint(-1,-1);
};

#endif //EOS_SINGLEPICTURECANVAS_H
