/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 09:15:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 09:15:51
 * @FilePath: \EOS\interface\include\window\camerabox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_CAMERABOX_H
#define EOSI_CAMERABOX_H

#include "window.h"
#include "cameratool.h"
#include "channelbox.h"

class INTERFACE_IMEXPORT CameraBox : public GroupBox
{
    Q_OBJECT
public:
    explicit CameraBox(QWidget*parent= nullptr);
    void setVisible(int index,bool visible);
    void setEnabled(int index,bool enabled);
    CameraInfo cameraInfo() const;
private:
    CameraTool * cameratool1;
    CameraTool * cameratool2;
    CameraTool * cameratool3;
    CameraTool * cameratool4;
    ChannelBox * channelbox;
signals:
    void phExposureChanged(int ms); // 1ms-15s
    void phGainChanged(int percent); // 100%-5000%
    void phBrightChanged(int bright); // 0-100
    void gfpExposureChanged(int ms);
    void gfpGainChanged(int percent);
    void gfpBrightChanged(int bright);
    void rfpExposureChanged(int ms);
    void rfpGainChanged(int percent);
    void rfpBrightChanged(int bright);
    void dapiExposureChanged(int ms);
    void dapiGainChanged(int percent);
    void dapiBrightChanged(int bright);
    void channelChanged(QVector<bool> states);
};
#endif //EOSI_CAMERABOX_H
