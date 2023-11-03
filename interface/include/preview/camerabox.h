/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 09:15:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-02 17:31:56
 * @FilePath: \EOS\interface\include\preview\camerabox.h
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
    CameraInfo saveInfo() const;
private:
    CameraTool * cameratool;
    Label * currentchannel;
    PushButton * savebtn;
    PushButton * capturebtn;
    PushButton * combinebtn;
    MultiCameraInfo camerainfo;
private slots:
    void onSaveBtn();
    void onCombineBtn();
    void onCaptureBtn();
    void adjustCamera();
signals:
    void cameraInfoChanged(const MultiCameraInfo &);
    void photoTaking();
    void cameraAdjusted(int exposure,int gain, int bright);
};
#endif //EOSI_CAMERABOX_H
