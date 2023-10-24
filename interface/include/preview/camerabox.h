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
#include "focusbox.h"

class INTERFACE_IMEXPORT CameraBox : public GroupBox
{
    Q_OBJECT
public:
    explicit CameraBox(QWidget*parent= nullptr);
    void setEnabled(bool enabled);
    void setChannel(int option);
    MultiCameraInfo cameraInfo() const;
private:
    CameraTool * cameratool;
    Label * currentchannel;
    PushButton * savebtn;
    PushButton * capturebtn;
    PushButton * combinebtn;
    MultiCameraInfo camerainfo;

    void onSaveBtn();
    CameraInfo saveInfo() const;
signals:
    void exposureChanged(int ms); // 1ms-15s
    void gainChanged(int percent); // 100%-5000%
    void brightChanged(int bright); // 0-100
    void infoChanged(const MultiCameraInfo &);
};
#endif //EOSI_CAMERABOX_H
