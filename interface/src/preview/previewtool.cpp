/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:53:52
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-02 17:36:35
 * @FilePath: \EOS\interface\src\preview\previewtool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewtool.h"

PreviewTool::PreviewTool(QWidget *parent) : QWidget(parent)
{
    historybox = new HistoryBox;
    wellbox = new WellBox;
    objectivebox = new ObjectiveBox;
    focusbox = new FocusBox;
    channelbox = new ChannelBox;
    camerabox = new CameraBox;
    lensbox = new LensBox;
    timebox = new TimeBox;
    zstackbox = new ZStackBox;
    savebox = new SaveBox;

    lay = new QVBoxLayout;
    lay->addWidget(historybox);
    lay->addWidget(wellbox);
    lay->addWidget(objectivebox);
    lay->addWidget(focusbox);
    lay->addWidget(channelbox);
    lay->addWidget(camerabox);
    lay->addWidget(lensbox);
    lay->addWidget(timebox);
    lay->addWidget(zstackbox);
    lay->addWidget(savebox);
    setLayout(lay);

    // 1. 信号直连
    connect(wellbox,&WellBox::wellbrandChanged,this,&PreviewTool::wellbrandChanged);
    connect(wellbox,&WellBox::manufacturerChanged,this,&PreviewTool::manufacturerChanged);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,this,&PreviewTool::objectiveChanged);
    connect(camerabox,&CameraBox::cameraInfoChanged,this,&PreviewTool::cameraInfoChanged);
    connect(camerabox,&CameraBox::photoTaking,this,&PreviewTool::photoTaking);
    connect(camerabox,&CameraBox::cameraAdjusted,this,&PreviewTool::cameraAdjusted);
    connect(channelbox,&ChannelBox::channelChanged,this,&PreviewTool::channelChanged);
    connect(zstackbox,&ZStackBox::zstackChanged,this,&PreviewTool::zstackChanged);
    connect(zstackbox,&ZStackBox::stitchChanged,this,&PreviewTool::stitchChanged);
    connect(savebox,&SaveBox::exportFilePath,this,&PreviewTool::exportFilePath);
    // 2. 信号槽函数
    connect(channelbox,&ChannelBox::channelChanged,camerabox,&CameraBox::setChannel);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,channelbox,&ChannelBox::disableChannel);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,timebox,&TimeBox::disableChannel);
    // 3.外部信号
    connect(this,&PreviewTool::objectiveSettingChanged,objectivebox,&ObjectiveBox::onObjectiveSettingChanged);
}

PreviewToolInfo PreviewTool::toolInfo() const
{
    PreviewToolInfo info;

    // 1. wellbox的brand+manufacturer
    auto wellinfo = wellbox->wellInfo();
    info[WellsizeField] = wellinfo[WellsizeField];
    info[BrandField] = wellinfo[BrandField];
    info[ManufacturerField] = wellinfo[ManufacturerField];
//    foreach(auto key,wellinfo.keys())
//        info[key] = wellinfo[key];

    // 2. objective
    auto objectiveinfo = objectivebox->objectiveInfo();
    info[CameraLocationField] = objectiveinfo[CameraLocationField];// 用了哪个位置的镜头
    info[ObjectiveField] = objectiveinfo[ObjectiveField]; // br4x
    info[ObjectiveMagnificationField] = objectiveinfo[ObjectiveMagnificationField]; // 4x,只放物镜的倍数,方便更新view窗口的尺寸
    info[ObjectiveTypeField] = objectiveinfo[ObjectiveTypeField];

    // 3. channel
    auto channelinfo = channelbox->channelInfo();//这个属于临时信息,其实没用(拍照这里有用,当前拍的通道)
    info[CurrentChannelField] = channelinfo[CurrentChannelField]; // 实际字符串 PH

    // 4. camerainfo,分不同通道,保存了gain,exposure,bright
    auto camerainfo = camerabox->cameraInfo();
    QVariant v;
    v.setValue(camerabox->saveInfo());
    info[CurrentInfoField] = v;

    QStringList channels;//所有保存过相机配置的通道
    if (!camerainfo.isEmpty()) { // 有保存过的通道参数
        foreach(auto channel, camerainfo.keys()) { // channel=PH,BR
            CameraInfo val = camerainfo[channel];// 每个通道的信息

            QVariantMap m;//把每个通道的信息转成QVarintMap
            m[ChannelField] = channel;
            m[ExposureField] = val[ExposureField];
            m[GainField] = val[GainField];
            m[BrightField] = val[BrightField];
            info[channel] = m; // 这个channel字段存储这个CameraInfo,"PH":m
            channels << channel;
        }
    }
    // 存储有哪些通道被设置过,方便信息组装,本身也是临时信息,没有用
    info[CaptureChannelField] = channels;//增加一个key=capture_channel,方便组装时判断

    // 5. zstack,stitch
    auto zstackinfo = zstackbox->zstackInfo();
    info[ZStackField] = zstackinfo[ZStackField];
    info[StitchField] = zstackinfo[StitchField];

    // 6. focus
    auto focus = focusbox->focus();
    auto focusstep = focusbox->focusStep();
    info[FocusField] = QString::number(focus);
    info[FocusStepField] = QString::number(focusstep);

    // 7. exper
    auto experinfo = timebox->timeInfo();
    foreach(auto key,experinfo.keys())
            info[key] = experinfo[key];

    // 8.

    //LOG<<"tool info = "<<info;
    return info;
}