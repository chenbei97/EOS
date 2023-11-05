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
    static ToupCamera instance;
    return instance;
}

ToupCamera::ToupCamera(QObject*parent):QObject(parent)
{
    devicelist = new ToupcamDeviceV2 [TOUPCAM_MAX];
    devicecount = Toupcam_EnumV2(devicelist);
    LOG<<"camera's count = "<<devicecount;
    if (devicecount) {
        camera = &devicelist[0];
        toupcam = Toupcam_Open(camera->id); // id是w_char类型

        auto resolu = resolution();
        imgdata = QSharedPointer<uchar> // 3通道,每通道8bit
                (new uchar[TDIBWIDTHBYTES(resolu.width() * 24) * resolu.height()]);

        setByteOrder(0);
        byteOrder();

        setExposureOption(1);
        exposureOption();

        getExposureRange();
        setExposure(100);
        exposure();

        getGainRange();
        setGain(100);
        gain();
    }
}

QSize ToupCamera::resolution() const
{
    if (!haveCamera()) return QSize(-1,-1);

    // 有相机的话,camera/toupcam不是nullptr
    //获取视频分辨率的索引,0=(2048,1536),1=(1024,768),2=(680,510)
    unsigned resolutionIndex = -1;
    Toupcam_get_eSize(toupcam, (unsigned*)&resolutionIndex);
    if (resolutionIndex < 0) return QSize(-1,-1); // 防止越界

    // ToupcamResolution存了该相机的分辨率
    unsigned imgWidth = camera->model->res[resolutionIndex].width;
    unsigned imgHeight = camera->model->res[resolutionIndex].height;

    LOG<<"resolution = "<<QSize(imgWidth,imgHeight);

    return QSize(imgWidth,imgHeight);
}

unsigned ToupCamera::cameraCount() const
{
    return devicecount;
}

bool ToupCamera::haveCamera() const
{
    return cameraCount() != 0;
}

void ToupCamera::setByteOrder(int option)
{ // 0-RGB 1-BGR
    if (option != 0 || option != 1 || !toupcam) return;

    Toupcam_put_Option(toupcam, TOUPCAM_OPTION_BYTEORDER, option);
}

int ToupCamera::byteOrder() const
{
    if (!toupcam) return -1;

    int option = 0;
    Toupcam_get_Option(toupcam, TOUPCAM_OPTION_BYTEORDER, &option);
    option?LOG<<"image is rgb":LOG<<"image is bgr";
}

void ToupCamera::setExposureOption(int option)
{// 0-禁止曝光 1-自动连续曝光 2-只曝光1次
    if (option != 0 || option != 1 || option != 2 || !toupcam) return;

    Toupcam_put_AutoExpoEnable(toupcam, option);
}

int ToupCamera::exposureOption() const
{
    if (!toupcam) return -1;

    int expoenable = 0;
    Toupcam_get_AutoExpoEnable(toupcam,&expoenable);
    LOG<<"image's exposure option = "<<expoenable;
}

void ToupCamera::setExposure(unsigned exp)
{
    if (!toupcam) return;

    Toupcam_put_ExpoTime(toupcam, exp);
}

unsigned ToupCamera::exposure() const
{
    if (!toupcam) return 0;

    unsigned exp = 0;
    Toupcam_get_ExpoTime(toupcam, &exp);
    LOG<<"current exposure is "<<exp;
    return exp;
}

ValueRangeUnSigned ToupCamera::getExposureRange() const
{
    ValueRangeUnSigned s = {0,0,0};
    if (!toupcam) return s;

    Toupcam_get_ExpTimeRange(toupcam,
                             &s.min,
                             &s.max,
                             &s.def);
    LOG<<"exposure range: "<<s;
    return s;
}

void ToupCamera::setGain(ushort gain)
{
    if (!toupcam) return;

    Toupcam_put_ExpoAGain(toupcam, gain);
}

ushort ToupCamera::gain() const
{
    if (!toupcam) return 0;

    ushort g = 0;
    Toupcam_get_ExpoAGain(toupcam, &g);
    LOG<<"current gain is "<<g;
    return g;
}

ValueRangeUShort ToupCamera::getGainRange() const
{
    ValueRangeUShort s = {0,0,0};
    if (!toupcam) return s;

    Toupcam_get_ExpoAGainRange(toupcam,
                               &s.min,
                               &s.max,
                               &s.def);
    LOG<<"gain range: "<<s;
    return s;
}