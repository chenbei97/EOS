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
    Toupcam_GigeEnable(nullptr, nullptr);// 初始化对相机的支持
    connect(this, &ToupCamera::evtCallback, this, &ToupCamera::processCallback);
}

void ToupCamera::eventCallBack(unsigned int nEvent, void *ctxEvent)
{ // 事件回调函数
    //Q_ASSERT(ctxEvent == ToupCameraPointer);
    ToupCamera* pThis = reinterpret_cast<ToupCamera*>(ctxEvent);
    emit pThis->evtCallback(nEvent);
    //ToupCameraPointer->evtCallback(nEvent);//必须强转不能用这个
}

void ToupCamera::processCallback(unsigned int nEvent)
{
    if (!toupcam) return;
    switch (nEvent) {
        case TOUPCAM_EVENT_IMAGE:
            captureLiveImage();
            break;
        case TOUPCAM_EVENT_STILLIMAGE:
            captureStillImage();
            break;
        case TOUPCAM_EVENT_EXPOSURE:
            break;
        case TOUPCAM_EVENT_TEMPTINT:
            break;
        case TOUPCAM_EVENT_DISCONNECTED:
            break;
        case TOUPCAM_EVENT_ERROR:
            break;
    }
}

QImage ToupCamera::capture()
{
    QImage img;
    if (toupcam) {
        if (imgdata) {
            auto res = resolution();
            img = QImage(imgdata.get(), res.width(), res.height(), QImage::Format_RGB888);
        }
    }
    return img;
}

void ToupCamera::captureLiveImage()
{
    if (!toupcam) return;

    int bStill = 0; // 拉取图像要求设置为0
    int bits = rgbBit(); // 默认就是24bit

    int rowPitch = 0; // 默认的填充方法 对于24bit 使用TDIBWIDTHBYTES(24*width)*height填充
    ToupcamFrameInfoV3 info;

    //auto resolu = resolution();
    //std::vector<uchar> vec(TDIBWIDTHBYTES(resolu.width() * 24) * resolu.height());

    if (SUCCEEDED(Toupcam_PullImageV3(toupcam, imgdata.get(),
                                      bStill, bits, rowPitch, &info))){
        //print_imageInfo(&info);

        // imgdata分配了多大内存读取就使用多大内存,_msize可以计算分配的内存
        //auto image = QImage::fromData(imgdata.get(), _msize(imgdata.get()));
        auto image = QImage(imgdata.get(), info.width, info.height, QImage::Format_RGB888);

        //把数据发出去,别的地方使用
        //auto pair = qMakePair(image,info);
        //emit imageCaptured(pair);
        emit imageCaptured(image);
    } else {
        LOG<<"pull image failed";
    }
//    unsigned width = 0, height = 0;
//    if (SUCCEEDED(Toupcam_PullImage(toupcam, imgdata.get(), 24, &width, &height)))
//    {
//        QImage image(imgdata.get(), width, height, QImage::Format_RGB888);
//        LOG<<image.size();
//    }
}

void ToupCamera::captureStillImage()
{
//    if (!toupcam) return;
//    int bits = rgbBit(); // 默认就是24bit
//    ToupcamFrameInfoV2 info;
//    if (SUCCEEDED(Toupcam_PullStillImageV2(toupcam,imgdata.get(),bits,&info))) {
//        QImage image(imgdata.get(), info.width, info.height, QImage::Format_RGB888);
//        //
//    }
}

void ToupCamera::print_imageInfo(ToupcamFrameInfoV3* info)
{
    LOG<<"w = "<<info->width<<" h = "<<info->height
    <<"exp = "<<info->expotime<<" gain = "<<info->expogain;
}

void ToupCamera::openCamera()
{
    if (toupcam){
        closeCamera();
    }
    devicelist = new ToupcamDeviceV2 [TOUPCAM_MAX];
    devicecount = Toupcam_EnumV2(devicelist);
    if (devicecount > 0) {
        camera = &devicelist[0];
        LOG<<"open camera id = "<<camera->id;
        toupcam = Toupcam_Open(camera->id); // id是w_char类型
        if (toupcam) {
            setRgbBit(0); // 24bit
            setByteOrder(0); // rgb
            setExposureOption(1); // auto continuity exposure
            setExposure(244);
            setGain(120);
            allocateImageBuffer();
            if (SUCCEEDED(Toupcam_StartPullModeWithCallback(toupcam,eventCallBack,this))){
                LOG<<"start camera successful!";
            } else {
                LOG<<"start camera failed!";
                closeCamera();
            }
        }
    }

}

void ToupCamera::closeCamera()
{
    //LOG<<"toupcam = "<<toupcam<<" imgdata = "<<imgdata;
    LOG<<"close camera";
    if (toupcam)
    {
        Toupcam_Close(toupcam);
        //delete toupcam; // close时已经释放了无需手动释放
        toupcam = nullptr;
    }
    if (imgdata) {

        if (imgdata.get()) {
            //delete imgdata.get(); // 不需要手动去删除,智能指针会在需要的时候删除的
            imgdata = nullptr;
        }
    }
    devicecount = 0;
    if (devicelist) {
        delete devicelist;
        devicelist = nullptr;
    }
}

unsigned ToupCamera::cameraCount() const
{
    return Toupcam_EnumV2(devicelist);
}

bool ToupCamera::haveCamera() const
{
    return cameraCount() != 0;
}

void ToupCamera::setRgbBit(int option)
{ // 0(RGB24)|1(RGB48) |2(RGB32)|3 (Grey8)|4 (Grey16)|5(RGB64)
    if (option < 0 || option > 5 || !toupcam) return;

    Toupcam_put_Option(toupcam, TOUPCAM_OPTION_RGB, option);
}

int ToupCamera::rgbBit() const
{
    if (!toupcam) return -1;

    int option = 0;
    Toupcam_get_Option(toupcam, TOUPCAM_OPTION_RGB, &option);
    int bits = 24;
    switch (option) {
        case 0:
            bits = 24;
            break;
        case 1:
            bits = 32;
            break;
        case 2:
            bits = 48;
            break;
        case 3:
            bits = 8;
            break;
        case 4:
            bits = 16;
            break;
        case 5:
            bits = 64;
            break;
    }
    return bits;
    //LOG<<"image rgb's option is "<<option;
}

void ToupCamera::allocateImageBuffer()
{ // 根据选择的bits/TOUPCAM_OPTION_RGB 来分配内存空间
    /***
    bits: 24 (RGB24), 32 (RGB32), 48 (RGB48), 8 (Grey), 16 (Grey), 64 (RGB64)
        在RAW模式下，此参数将被忽略。
        bits=0表示使用基于TOUPACM_OPTION_RGB的默认位 也就是RGB24
        当bits和TOUPACM_OPTION_RGB不一致时，将不得不执行格式转换，从而导致效率损失。
        TOUPCAM_OPTION_RGB可选: 0(RGB24)|1(RGB48) |2(RGB32)|3 (Grey8)|4 (Grey16)|5(RGB64),也就是说bits要和TOUPCAM_OPTION_RGB一致
        Toupcam_put_Option(hcam, TOUPCAM_OPTION_RGB, 0);
      rowPitch: 从一行到下一行的距离。rowPitch=0表示使用默认的行间距。rowPitch=-1表示零填充
      | format                             | 0 means default row pitch     | -1 means zero padding |
      | RGB       | RGB24                  | TDIBWIDTHBYTES(24 * Width)    | Width * 3             |
      |           | RGB32                  | Width * 4                     | Width * 4             |
      |           | RGB48                  | TDIBWIDTHBYTES(48 * Width)    | Width * 6             |
      |           | GREY8                  | TDIBWIDTHBYTES(8 * Width)     | Width                 |
      |           | GREY16                 | TDIBWIDTHBYTES(16 * Width)    | Width * 2             |
      |           | RGB64                  | Width * 8                     | Width * 8             |
   ***/
    if (!toupcam) return;

    auto resolu = resolution();

    auto bits = rgbBit();
    //LOG<<"bits = "<<bits;
    // width取10,带入(240+31)&(~31) /8 忽略低5bit 然后再除8,忽略低5bit就是10000=16一定可以被8整除
    switch (bits) { // 根据位数来分配对应的buffer_size存储图片
        case 24: // RGB24
            imgdata = QSharedPointer<uchar> // TDIBWIDTHBYTES(24 * Width) TDIBWIDTHBYTES的含义是忽略低5bit取整
                    (new uchar[TDIBWIDTHBYTES(resolu.width() * 24) * resolu.height()]);
            //LOG<<TDIBWIDTHBYTES(resolu.width() * 24)<<resolu.height()<<resolu.width();
            break;
        case 32: // RGB32
            imgdata = QSharedPointer<uchar> // Width * 4
                    (new uchar[resolu.width() * 4 * resolu.height()]);
            break;
        case 48: // RGB48
            imgdata = QSharedPointer<uchar> // TDIBWIDTHBYTES(48 * Width)
                    (new uchar[TDIBWIDTHBYTES(resolu.width() *48) * resolu.height()]);
            break;
        case 8: // GREY8
            imgdata = QSharedPointer<uchar> // TDIBWIDTHBYTES(8 * Width)
                    (new uchar[TDIBWIDTHBYTES(resolu.width() * 8) * resolu.height()]);
            break;
        case 16: // GREY16
            imgdata = QSharedPointer<uchar> // TDIBWIDTHBYTES(16 * Width)
                    (new uchar[TDIBWIDTHBYTES(resolu.width() * 16) * resolu.height()]);
            break;
        case 64: // RGB64
            imgdata = QSharedPointer<uchar> // 8 * Width
                    (new uchar[resolu.width() * 8 * resolu.height()]);
            break;
    }

}

QSize ToupCamera::resolution() const
{
    if (!haveCamera()) return QSize(-1,-1);

    // 有相机的话,camera/toupcam不是nullptr
    //获取视频分辨率的索引,0=(2048,1536),1=(1024,768),2=(680,510)
    unsigned resolutionIndex = -1;
    Toupcam_get_eSize(toupcam, &resolutionIndex);
    if (resolutionIndex < 0) return QSize(-1,-1); // 防止越界

    // ToupcamResolution存了该相机的分辨率
    unsigned imgWidth = camera->model->res[resolutionIndex].width;
    unsigned imgHeight = camera->model->res[resolutionIndex].height;

    //LOG<<"resolution = "<<QSize(imgWidth,imgHeight);

    return QSize(imgWidth,imgHeight);
}

double ToupCamera::frameRate() const
{
    double rate = 0.0;
    unsigned nFrame = 0, nTime = 0, nTotalFrame = 0;
    if (toupcam && SUCCEEDED(Toupcam_get_FrameRate(toupcam, &nFrame, &nTime, &nTotalFrame)) && (nTime > 0))
    {
        rate = nFrame * 1000.0 / nTime;
    }
    return rate;
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