/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:06:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-02 14:08:52
 * @FilePath: \EOS\interface\src\preview\preview_tcp.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

void Preview::takingPhoto()
{
    auto toolinfo = toolbar->toolInfo();
    auto patterninfo = pattern->patternInfo();
    previewinfo[PreviewToolField] = toolinfo;
    previewinfo[PreviewPatternField] = patterninfo;
    //LOG<<"preview info = "<<previewinfo;

    auto current_channel = toolinfo[CurrentChannelField].toString();
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    //auto save_channels = toolinfo[CaptureChannelField].toStringList();//保存过设置的所有通道

    QVariantMap m;

    m[ChannelField] = getIndexFromFields(current_channel);
    m[BrightField] = current_info[BrightField];
    int exposure = current_info[ExposureField].toUInt();
    int gain = current_info[GainField].toUInt();

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
    LOG<<exposure<<gain<<m[BrightField]<<m[ChannelField]<<msg;
    SocketPointer->exec(TcpFramePool.frame0x0004,msg,true);

    // 等待回复后调用相机拍照
    if (ParserResult.toBool()) {
        LOG<<"灯成功打开"; // 做这些事
    }

    // 拍照结束后回复拍照结束
}

void Preview::previewView(const QPoint &point)
{
    
}

void Preview::saveExperConfig(const QString& path)
{ // 保存实验配置
    //LOG<<path<<previewinfo;
    previewinfo[PreviewPatternField] = pattern->patternInfo();
    previewinfo[PreviewToolField] = toolbar->toolInfo();

    AssemblerPointer->assemble(TcpFramePool.frame0x0001,previewinfo);
    auto json = AssemblerPointer->message();

    json.chop(3);//删掉尾缀@@@

    JsonReadWrite m; // 借助工具类写到文件内
    m.writeJson(path,json);
}