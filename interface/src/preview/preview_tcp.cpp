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

    if (current_channel.isEmpty()) {
        LOG<<"toggle channel isvalid";
        Q_ASSERT(option == -1); // 如果没有通道,此时是空字符串,切option=1,那么这个时候就直接关灯即可
        // 不过关灯需要知道关哪个灯,这个工作在channelboxx已经做了,这里什么也不做
        return; // 灯全灭的情况直接返回
    }

    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();

    QVariantMap m;
    m[CurrentChannelField] = getIndexFromFields(current_channel);
    m[BrightField] = current_info[BrightField];
    m[TurnOffLight] = 0;

    AssemblerPointer->assemble(TcpFramePool.frame0x0006,m);
    auto msg = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.frame0x0006,msg, true);

    if (ParserResult.toBool()) {
        LOG<<"开灯! 当前开灯通道为 "<<m[CurrentChannelField];
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

    if (current_channel.isEmpty()) {
        LOG<<"have no channel";
        return; // 如果通道无效,没有开灯,调节参数没有意义,不发给下位机
    }

    // 滑动条速度很快,这里组装不再通过Assembler来组装,可能同步会出问题,直接组装
    QJsonObject object;
    object[FrameField] = TcpFramePool.frame0x0005;
    object[Field0x0005.bright] = br;
    object[Field0x0005.current_channel] = getIndexFromFields(current_channel);
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());;

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

void Preview::showCapturedImage(const QImage& image)
{
    auto img = image.scaled(livecanvas->size(),Qt::KeepAspectRatio,Qt::FastTransformation);
    QVariantMap m;
    m[ImageField] = QPixmap::fromImage(img);
    livecanvas->setData(m);
}

void Preview::takingPhoto()
{
    auto toolinfo = toolbar->toolInfo();
//    auto patterninfo = pattern->patternInfo();
//    previewinfo[PreviewToolField] = toolinfo;
//    previewinfo[PreviewPatternField] = patterninfo;

    //auto current_channel = toolinfo[CurrentChannelField].toString();
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();

    QVariantMap m;

    //m[CurrentChannelField] = getIndexFromFields(current_channel);
    m[BrightField] = current_info[BrightField];
    int exp = current_info[ExposureField].toUInt();
    int ga = current_info[GainField].toUInt();

//auto save_channels = toolinfo[CaptureChannelField].toStringList();//保存过设置的所有通道
// 不从保存过的参数去拿,而是从UI的信息直接去拿
//    if (save_channels.contains(current_channel)) {
//        // 例如当前通道PH,设置过PH的相机参数
//        auto camera_info = toolinfo[current_channel].value<QVariantMap>();
//        Q_ASSERT(camera_info[ChannelField].toString() == current_channel);
//        exp = camera_info[ExposureField].toUInt();
//        ga = camera_info[GainField].toUInt();
//        m[BrightField] = camera_info[BrightField];
//    }

    AssemblerPointer->assemble(TcpFramePool.frame0x0004,m);
    auto msg = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.frame0x0004,msg,true);
    // 等待回复后调用相机拍照
    if (ParserResult.toBool()) {

#ifndef usetoupcamera
        ToupCameraPointer->setExposure(exp);
        ToupCameraPointer->setGain(ga);
        auto pix = ToupCameraPointer->capture();
#else
        setExposure(exp);
        setGain(ga);
        auto pix = capture();
#endif
        LOG<<"已经调整亮度为 "<<current_info[BrightField].toInt()
        <<" 曝光和增益为 "<<ToupCameraPointer->exposure()<<ToupCameraPointer->gain();

        QVariantMap m;
        m[ImageField] = pix;
        photocanvas->setData(m);

    }
}

void Preview::previewViewByClickView(const QPoint &viewpoint)
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
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];//点孔触发预览的时候需要传递viewsize
    auto holecoordinate = viewpattern->currentViewInfo()[HoleCoordinateField].toPoint(); // 这个信息单独点击孔是没有传递的
    //auto current_channel = getIndexFromFields(toolinfo[CurrentChannelField].toString());
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    auto bright = current_info[BrightField];

    // 自己需要的相机参数
    int exposure = current_info[ExposureField].toUInt();
    int gain = current_info[GainField].toUInt();
    //LOG<<wellsize<<viewpoint<<holecoordinate<<bright<<current_channel;
    ToupCameraPointer->setExposure(exposure);
    ToupCameraPointer->setGain(gain);

    QVariantMap m;
    m[ObjectiveField] = objective;
    m[BrandField] = brand;
    m[ManufacturerField] = manufacturer;
    m[WellsizeField] = wellsize;
    m[HoleViewSizeField] = viewsize;
    m[HoleCoordinateField] = holecoordinate;
    m[ViewCoordinateField] = viewpoint;
    //m[CurrentChannelField] = current_channel;// 不需要再发当前通道了
    m[BrightField] = bright;
    m[IsHoleField] = 0;

    AssemblerPointer->assemble(TcpFramePool.frame0x0000,m);
    auto msg = AssemblerPointer->message();
    SocketPointer->exec(TcpFramePool.frame0x0000,msg, true);
    if (ParserResult.toBool()) {
        LOG<<"已经移动电机到指定视野坐标 "<<viewpoint;
    }
}

void Preview::previewViewByClickHole(const QPoint &holepoint)
{
    if (holepoint == QPoint(-1,-1)) return;

    auto toolinfo = toolbar->toolInfo();
//    auto patterninfo = pattern->patternInfo();
//    previewinfo[PreviewToolField] = toolinfo;
//    previewinfo[PreviewPatternField] = patterninfo;

    // 预览事件需要的参数
    auto objective = getIndexFromFields(toolinfo[ObjectiveMagnificationField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto wellsize = toolinfo[WellsizeField].toUInt();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];//点孔触发预览的时候需要传递viewsize
    //auto holecoordinate = viewpattern->currentViewInfo()[HoleCoordinateField].toPoint(); // 这个信息单独点击孔是没有传递的
    auto holecoordinate = holepoint; // 所以才为什么只能分成previewViewByClickHole和previewViewByClickView 2个函数写了
    //auto current_channel = getIndexFromFields(toolinfo[CurrentChannelField].toString());
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    auto bright = current_info[BrightField];

    // 自己需要的相机参数
    int exposure = current_info[ExposureField].toUInt();
    int gain = current_info[GainField].toUInt();
    ToupCameraPointer->setExposure(exposure);
    ToupCameraPointer->setGain(gain);

    QVariantMap m;
    m[ObjectiveField] = objective;
    m[BrandField] = brand;
    m[ManufacturerField] = manufacturer;
    m[WellsizeField] = wellsize;
    m[HoleViewSizeField] = viewsize;
    m[HoleCoordinateField] = holecoordinate;
    m[ViewCoordinateField] = QPoint(-1,-1);//点孔触发视野坐标没有意义
    //m[CurrentChannelField] = current_channel; // 不需要再发当前通道了
    m[BrightField] = bright;
    m[IsHoleField] = 1;

    AssemblerPointer->assemble(TcpFramePool.frame0x0000,m);
    auto msg = AssemblerPointer->message();
    SocketPointer->exec(TcpFramePool.frame0x0000,msg, true);
    if (ParserResult.toBool()) {
        LOG<<"已经移动电机到指定孔坐标 "<<holepoint;
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