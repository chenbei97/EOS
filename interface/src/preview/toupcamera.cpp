/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 16:03:32
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 16:06:56
 * @FilePath: \EOS\interface\include\preview\toupcamera.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "toupcamera.h"

ToupCamera& ToupCamera::instance()
{
    static ToupCamera camera;
    return camera;
}

ToupCamera::ToupCamera(QObject*parent):QObject(parent)
{

}

bool ToupCamera::haveCamera() const
{
    ToupcamDeviceV2 arr[TOUPCAM_MAX] = {0};
    unsigned count = Toupcam_EnumV2(arr);
    if (!count)
        LOG<<"have no camera";
    return count != 0;
}