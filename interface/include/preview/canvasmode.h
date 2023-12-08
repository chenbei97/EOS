/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 13:59:06
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 13:59:57
 * @FilePath: \EOS\interface\include\preview\cameramode.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_CANVASMODE_H
#define EOSI_CANVASMODE_H

#include "window.h"

class INTERFACE_IMEXPORT CanvasMode: public QWidget
{
    Q_OBJECT
public:
    enum canvasMode {LiveMode,PhotoMode,VideoMode};
    explicit CanvasMode(QWidget*parent= nullptr);
    void changeMode(canvasMode mode);
    void paintEvent(QPaintEvent*event) override;
    QSize sizeHint() const override;
    QSize  minimumSizeHint() const override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    canvasMode canvasmode = LiveMode;
signals:
    void cameraModeChanged(int option);
};
#endif //EOSI_CANVASMODE_H
