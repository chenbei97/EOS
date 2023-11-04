/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 16:03:32
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 16:06:56
 * @FilePath: \EOS\interface\include\preview\toupcamera.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#ifndef EOSI_TOUPCAMERA_H
#define EOSI_TOUPCAMERA_H

#include "window.h"
#include "toupcam.h"

class INTERFACE_IMEXPORT ToupCamera: public QObject
{
    Q_OBJECT
public:
    static ToupCamera& instance();
    bool haveCamera() const;
private:
    HToupcam toupcam = nullptr;
    ToupcamDeviceV2 camera;
    uchar* imagedata = nullptr;
private:
    explicit ToupCamera(QObject*parent= nullptr);
};

#define  ToupCameraPointer (&ToupCamera::instance())
#endif //EOSI_TOUPCAMERA_H
