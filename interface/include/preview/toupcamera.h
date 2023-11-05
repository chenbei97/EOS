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

typedef QPair<QImage,ToupcamFrameInfoV3> ImageInfo;

class INTERFACE_IMEXPORT ToupCamera: public QObject
{
    Q_OBJECT
public:
    static ToupCamera& instance();

    void openCamera();
    void closeCamera();

    bool haveCamera() const;
    unsigned cameraCount() const;

    ImageInfo capture();

    void print_imageInfo(ToupcamFrameInfoV3* info);

    QSize resolution() const;

    void setByteOrder(int option);
    int byteOrder() const;

    void setRgbBit(int option);
    int rgbBit() const;
    void allocateImageBuffer();

    void setExposureOption(int option);
    int exposureOption() const;

    ValueRangeUnSigned getExposureRange() const;
    void setExposure(unsigned exp);
    unsigned exposure() const;

    ValueRangeUShort getGainRange() const;
    void setGain(ushort gain);
    ushort gain() const;
private:
    HToupcam toupcam = nullptr;
    ToupcamDeviceV2 * camera = nullptr;
    //uchar* imagedata = nullptr;
    ToupcamDeviceV2 * devicelist = nullptr;
    unsigned devicecount = 0;
    QSharedPointer<uchar> imgdata = nullptr;
private:
    explicit ToupCamera(QObject*parent= nullptr);
};

#define  ToupCameraPointer (&ToupCamera::instance())
#endif //EOSI_TOUPCAMERA_H
