/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:06:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-02 14:08:52
 * @FilePath: \EOS\interface\src\preview\preview_tcp.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

void Preview::toggleChannel(int option)
{
    auto toolinfo = toolbar->toolInfo();

    auto current_channel = toolinfo[CurrentChannelField].toString();
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();

    QVariantMap m;
    m[CurrentChannelField] = getIndexFromFields(current_channel);
    m[BrightField] = current_info[BrightField];

    AssemblerPointer->assemble(TcpFramePool.frame0x0006,m);
    auto msg = AssemblerPointer->message();
    //LOG<<m[BrightField]<<m[CurrentChannelField]<<msg;

    SocketPointer->exec(TcpFramePool.frame0x0006,msg, true);

    if (ParserResult.toBool()) {
        LOG<<"切换通道到"<<m[CurrentChannelField];
    }
}

void Preview::adjustLens(int option)
{ // 0-left,1-rop,2-right,3-bottom

    // 点调节镜头
    QJsonObject object;
    object[FrameField] = TcpFramePool.frame0x0008;
    object[DirectionField] = option;
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());

    //LOG<<msg;
    SocketPointer->exec(TcpFramePool.frame0x0008,msg, true);

    if (ParserResult.toBool()) {
        LOG<<"移动镜头方向: "<<option;
    }
}

void Preview::adjustCamera(int exp,int gain,int br)
{
    auto toolinfo = toolbar->toolInfo();

    auto current_channel = toolinfo[CurrentChannelField].toString();

    // 滑动条速度很快,这里组装不再通过Assembler来组装,可能同步会出问题,直接组装
    QJsonObject object;
    object[FrameField] = TcpFramePool.frame0x0005;
    object[Field0x0005.bright] = br;
    object[Field0x0005.current_channel] = getIndexFromFields(current_channel);
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());;

    //LOG<<msg<<exp<<gain<<br;


    // 发送消息异步发送就行不需要同步,防止卡住
    SocketPointer->exec(TcpFramePool.frame0x0005,msg, false);


}

void Preview::onAdjustCamera(const QString & f,const QVariant & d)
{
    static int count = 0;
    if (d.toBool() && f == TcpFramePool.frame0x0005) {
        LOG<<"调整相机参数"<<++count<<"次"; // 做这些事
    }
}

void Preview::takingPhoto()
{
    auto toolinfo = toolbar->toolInfo();
//    auto patterninfo = pattern->patternInfo();
//    previewinfo[PreviewToolField] = toolinfo;
//    previewinfo[PreviewPatternField] = patterninfo;

    auto current_channel = toolinfo[CurrentChannelField].toString();
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();


    QVariantMap m;

    m[CurrentChannelField] = getIndexFromFields(current_channel);
    m[BrightField] = current_info[BrightField];
    int exposure = current_info[ExposureField].toUInt();
    int gain = current_info[GainField].toUInt();

//auto save_channels = toolinfo[CaptureChannelField].toStringList();//保存过设置的所有通道
// 不从保存过的参数去拿,而是从UI的信息直接去拿
//    if (save_channels.contains(current_channel)) {
//        // 例如当前通道PH,设置过PH的相机参数
//        auto camera_info = toolinfo[current_channel].value<QVariantMap>();
//        Q_ASSERT(camera_info[ChannelField].toString() == current_channel);
//        exposure = camera_info[ExposureField].toUInt();
//        gain = camera_info[GainField].toUInt();
//        m[BrightField] = camera_info[BrightField];
//    }

    AssemblerPointer->assemble(TcpFramePool.frame0x0004,m);
    auto msg = AssemblerPointer->message();
    //LOG<<exposure<<gain<<m[BrightField]<<m[CurrentChannelField]<<msg;

    SocketPointer->exec(TcpFramePool.frame0x0004,msg,true);
    // 等待回复后调用相机拍照
    if (ParserResult.toBool()) {
        LOG<<"灯成功打开"; // 做这些事

        ToupCameraPointer->setExposure(exposure);
        ToupCameraPointer->setGain(gain);
        ToupCameraPointer->capture();
    }

    // 拍照结束后回复拍照结束
    QVariantMap m1;
    m1[TurnOffLight] = "1";
    m1[CurrentChannelField] = getIndexFromFields(current_channel);
    AssemblerPointer->assemble(TcpFramePool.frame0x0007, m1);
    auto msg1 = AssemblerPointer->message();
    //LOG<<msg1;
    SocketPointer->exec(TcpFramePool.frame0x0007,msg, true);
    if (ParserResult.toBool()) {
        LOG<<"灯成功关闭";
    }
}

void Preview::previewView(const QPoint &viewpoint)
{
    if (viewpoint == QPoint(-1,-1)) return;

    auto toolinfo = toolbar->toolInfo();
//    auto patterninfo = pattern->patternInfo();
//    previewinfo[PreviewToolField] = toolinfo;
//    previewinfo[PreviewPatternField] = patterninfo;

    // 预览事件需要的参数
    auto objective = getIndexFromFields(toolinfo[ObjectiveMagnificationField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto wellsize = toolinfo[WellsizeField].toUInt();
    //auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];
    auto holecoordinate = viewpattern->currentViewInfo()[HoleCoordinateField].toPoint();
    auto current_channel = getIndexFromFields(toolinfo[CurrentChannelField].toString());
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    auto bright = current_info[BrightField];

    // 自己需要的相机参数
    int exposure = current_info[ExposureField].toUInt();
    int gain = current_info[GainField].toUInt();
    //LOG<<wellsize<<viewpoint<<holecoordinate<<bright<<current_channel;

    QVariantMap m;
    m[ObjectiveField] = objective;
    m[BrandField] = brand;
    m[ManufacturerField] = manufacturer;
    m[WellsizeField] = wellsize;
    m[HoleCoordinateField] = holecoordinate;
    m[ViewCoordinateField] = viewpoint;
    m[CurrentChannelField] = current_channel;
    m[BrightField] = bright;

    AssemblerPointer->assemble(TcpFramePool.frame0x0000,m);
    auto msg = AssemblerPointer->message();
    LOG<<msg;
    SocketPointer->exec(TcpFramePool.frame0x0000,msg, true);
    LOG<<ParserResult;
    if (ParserResult.toBool()) {
        LOG<<"预览点击到位";
    }
}

void Preview::saveExperConfig(const QString& path)
{ // 保存实验配置
    previewinfo[PreviewPatternField] = pattern->patternInfo();
    previewinfo[PreviewToolField] = toolbar->toolInfo();

    AssemblerPointer->assemble(TcpFramePool.frame0x0001,previewinfo);
    auto json = AssemblerPointer->message();

    json.chop(3);//删掉尾缀@@@

    JsonReadWrite m; // 借助工具类写到文件内
    m.writeJson(path,json);
}