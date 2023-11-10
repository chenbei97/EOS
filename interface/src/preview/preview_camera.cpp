/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-06 16:29:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-07 08:31:24
 * @FilePath: \EOS\interface\src\preview\preview_camera.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

#ifdef notusetoupcamera

void Preview::captureLiveImage()
{
    if (!toupcam) return;

    int bStill = 0; // 拉取图像要求设置为0
    int bits = rgbBit(); // 默认就是24bit

    int rowPitch = 0; // 默认的填充方法 对于24bit 使用TDIBWIDTHBYTES(24*width)*height填充
    ToupcamFrameInfoV3 info;

    if (SUCCEEDED(Toupcam_PullImageV3(toupcam, imgdata.get(),
                                      bStill, bits, rowPitch, &info))){
        auto image = QImage(imgdata.get(), info.width, info.height, QImage::Format_RGB888);
        //auto img = image.scaled(livecanvas->size(),Qt::KeepAspectRatio,Qt::FastTransformation);
        // 在这做图像缩放会卡,应该转移出去
#ifdef uselabelcanvas
        livecanvas->setPixmap(QPixmap::fromImage(image));
#else
        livecanvas->setImage(image);
#endif
    }
}

QImage Preview::capture()
{ // 拍照时获取一帧图像
    QImage img;
    if (toupcam) {
        if (imgdata) {
            auto image = QImage(imgdata.get(), cameraResolution.width(), cameraResolution.height(), QImage::Format_RGB888);
            img = image.scaled(photocanvas->size(),Qt::KeepAspectRatio,Qt::FastTransformation);
        }
    }
    return img;
}

void Preview::exposureEvent()
{
    unsigned time = 0;
    unsigned short gain = 0;
    Toupcam_get_ExpoTime(toupcam, &time);
    Toupcam_get_ExpoAGain(toupcam, &gain);

    toolbar->exposureGainCaptured(time,gain);
}

void Preview::setRgbBit(int option)
{ // 0(RGB24)|1(RGB48) |2(RGB32)|3 (Grey8)|4 (Grey16)|5(RGB64)
    if (option < 0 || option > 5 || !toupcam) return;

    Toupcam_put_Option(toupcam, TOUPCAM_OPTION_RGB, option);
}

int Preview::rgbBit() const
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

void Preview::allocateImageBuffer()
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

    //LOG<<"resolution = "<<cameraResolution;
    auto bits = rgbBit();
    //LOG<<"bits = "<<bits;
    // width取10,带入(240+31)&(~31) /8 忽略低5bit 然后再除8,忽略低5bit就是10000=16一定可以被8整除
    switch (bits) { // 根据位数来分配对应的buffer_size存储图片
        case 24: // RGB24
            imgdata = QSharedPointer<uchar> // TDIBWIDTHBYTES(24 * Width) TDIBWIDTHBYTES的含义是忽略低5bit取整
                    (new uchar[TDIBWIDTHBYTES(cameraResolution.width() * 24) * cameraResolution.height()]);
            //LOG<<TDIBWIDTHBYTES(resolu.width() * 24)<<resolu.height()<<resolu.width();
            break;
        case 32: // RGB32
            imgdata = QSharedPointer<uchar> // Width * 4
                    (new uchar[cameraResolution.width() * 4 * cameraResolution.height()]);
            break;
        case 48: // RGB48
            imgdata = QSharedPointer<uchar> // TDIBWIDTHBYTES(48 * Width)
                    (new uchar[TDIBWIDTHBYTES(cameraResolution.width() *48) * cameraResolution.height()]);
            break;
        case 8: // GREY8
            imgdata = QSharedPointer<uchar> // TDIBWIDTHBYTES(8 * Width)
                    (new uchar[TDIBWIDTHBYTES(cameraResolution.width() * 8) * cameraResolution.height()]);
            break;
        case 16: // GREY16
            imgdata = QSharedPointer<uchar> // TDIBWIDTHBYTES(16 * Width)
                    (new uchar[TDIBWIDTHBYTES(cameraResolution.width() * 16) * cameraResolution.height()]);
            break;
        case 64: // RGB64
            imgdata = QSharedPointer<uchar> // 8 * Width
                    (new uchar[cameraResolution.width() * 8 * cameraResolution.height()]);
            break;
    }

}

void Preview::setByteOrder(int option)
{ // 0-RGB 1-BGR
    if (option != 0 || option != 1 || !toupcam) return;

    Toupcam_put_Option(toupcam, TOUPCAM_OPTION_BYTEORDER, option);
}

int Preview::byteOrder() const
{
    if (!toupcam) return -1;

    int option = 0;
    Toupcam_get_Option(toupcam, TOUPCAM_OPTION_BYTEORDER, &option);
}

void Preview::setExposureOption(int option)
{// 0-禁止曝光 1-自动连续曝光 2-只曝光1次
    if (option != 0 || option != 1 || option != 2 || !toupcam) return;

    Toupcam_put_AutoExpoEnable(toupcam, option);
}

int Preview::exposureOption() const
{
    if (!toupcam) return -1;

    int expoenable = 0;
    Toupcam_get_AutoExpoEnable(toupcam,&expoenable);
}

void Preview::setExposure(unsigned exp)
{
    if (!toupcam) return;

    Toupcam_put_ExpoTime(toupcam, exp);
}

unsigned Preview::exposure() const
{
    if (!toupcam) return 0;

    unsigned exp = 0;
    Toupcam_get_ExpoTime(toupcam, &exp);
    return exp;
}

void Preview::setGain(ushort gain)
{
    if (!toupcam) return;

    Toupcam_put_ExpoAGain(toupcam, gain);
}

ushort Preview::gain() const
{
    if (!toupcam) return 0;

    ushort g = 0;
    Toupcam_get_ExpoAGain(toupcam, &g);
    return g;
}

bool Preview::isCameraOpen() const
{
    LOG<<"camera is open? "<<(toupcam!= nullptr);
    return toupcam != nullptr;
}

void Preview::openCamera()
{
    if (toupcam){
        closeCamera();
    }
    ToupcamDeviceV2 devicelist[TOUPCAM_MAX] ={0};
    auto devicecount = Toupcam_EnumV2(devicelist);
    if (devicecount == 1) {
        camera = devicelist[0];
        LOG<<"open camera id = "<<camera.id;
        toupcam = Toupcam_Open(camera.id); // id是w_char类型
        if (toupcam) {
            unsigned resolutionIndex;
            Toupcam_get_eSize(toupcam, &resolutionIndex);
            cameraResolution = QSize(camera.model->res[resolutionIndex].width,
                                     camera.model->res[resolutionIndex].height);
            setRgbBit(0); // 24bit
            setByteOrder(0); // rgb
            setExposureOption(0); // 初始设置是不使用自动曝光
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

void Preview::closeCamera()
{
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
            imgdata.clear();
            imgdata = nullptr;
        }
    }
}

unsigned Preview::cameraCount() const
{
    ToupcamDeviceV2 devicelist[TOUPCAM_MAX] ={0};
    auto devicecount = Toupcam_EnumV2(devicelist);
    return devicecount;
}

bool Preview::haveCamera() const
{
    return cameraCount() != 0;
}

void Preview::eventCallBack(unsigned int nEvent, void *ctxEvent)
{ // 事件回调函数
    //Q_ASSERT(ctxEvent == ToupCameraPointer);
    Preview* pThis = reinterpret_cast<Preview*>(ctxEvent);
    emit pThis->evtCallback(nEvent);
    //ToupCameraPointer->evtCallback(nEvent);//必须强转不能用这个
}

void Preview::processCallback(unsigned int nEvent)
{
    if (!toupcam) return;
    if (nEvent == TOUPCAM_EVENT_IMAGE)
        captureLiveImage();
}
#endif