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
private:
    CameraTool * cameratool1;
    CameraTool * cameratool2;
    CameraTool * cameratool3;
    CameraTool * cameratool4;
    ChannelBox * channelbox;
signals:
    void exposureChanged1(int ms); // 1ms-15s
    void gainChanged1(int percent); // 100%-5000%
    void brightChanged1(int bright); // 0-100
    void exposureChanged2(int ms);
    void gainChanged2(int percent);
    void brightChanged2(int bright);
    void exposureChanged3(int ms);
    void gainChanged3(int percent);
    void brightChanged3(int bright);
    void exposureChanged4(int ms);
    void gainChanged4(int percent);
    void brightChanged4(int bright);
};
#endif //EOSI_CAMERABOX_H
