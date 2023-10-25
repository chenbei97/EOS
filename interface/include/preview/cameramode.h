/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 13:59:06
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 13:59:57
 * @FilePath: \EOS\interface\include\preview\cameramode.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_CAMERAMODE_H
#define EOSI_CAMERAMODE_H

#include "window.h"

class INTERFACE_IMEXPORT CameraMode: public QWidget
{
    Q_OBJECT
public:
    enum cameraMode {LiveMode,PhotoMode};
    explicit CameraMode(QWidget*parent= nullptr);
    void changeMode(cameraMode mode);
    void paintEvent(QPaintEvent*event) override;
    QSize sizeHint() const override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    bool isLive = true;
signals:
    void cameraModeChanged(int option);
};
#endif //EOSI_CAMERAMODE_H
