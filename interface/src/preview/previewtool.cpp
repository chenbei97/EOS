/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:53:52
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:54:32
 * @FilePath: \EOS\interface\src\window\previewtool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewtool.h"

PreviewTool::PreviewTool(QWidget *parent) : QWidget(parent)
{
    wellbox = new WellBox;
    objectivebox = new ObjectiveBox;
    channelbox = new ChannelBox;
    camerabox = new CameraBox;

    lay = new QVBoxLayout;
    lay->addWidget(wellbox);
    lay->addWidget(objectivebox);
    lay->addWidget(channelbox);
    lay->addWidget(camerabox);
    setLayout(lay);

    connect(wellbox,&WellBox::wellbrandChanged,this,&PreviewTool::wellbrandChanged);
    connect(wellbox,&WellBox::wellsizeChanged,this,&PreviewTool::wellsizeChanged);
    connect(wellbox,&WellBox::welldishChanged,this,&PreviewTool::welldishChanged);
    connect(wellbox,&WellBox::wellflaskChanged,this,&PreviewTool::wellflaskChanged);
    connect(wellbox,&WellBox::wellslideChanged,this,&PreviewTool::wellslideChanged);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,this,&PreviewTool::objectiveChanged);
    connect(wellbox,&WellBox::wellbrandChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::wellsizeChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::welldishChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::wellflaskChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::wellslideChanged,this,&PreviewTool::infoChanged);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,this,&PreviewTool::infoChanged);


    connect(camerabox,&CameraBox::exposureChanged,this,&PreviewTool::exposureChanged);
    connect(camerabox,&CameraBox::gainChanged,this,&PreviewTool::gainChanged);
    connect(camerabox,&CameraBox::brightChanged,this,&PreviewTool::brightChanged);
    // 不需要滑动条移动就会触发信息改变,只能是保存相机信息那个动作才可以触发
    connect(camerabox,&CameraBox::infoChanged,this,&PreviewTool::infoChanged);
    //connect(camerabox,&CameraBox::exposureChanged,this,&PreviewTool::infoChanged);
    //connect(camerabox,&CameraBox::gainChanged,this,&PreviewTool::infoChanged);
    //connect(camerabox,&CameraBox::brightChanged,this,&PreviewTool::infoChanged);


    connect(channelbox,&ChannelBox::channelChanged,this,&PreviewTool::channelChanged);
    connect(channelbox,&ChannelBox::channelChanged,camerabox,&CameraBox::setChannel);
    connect(channelbox,&ChannelBox::channelChanged,this,&PreviewTool::infoChanged);

    //LOG<<"wellinfo = "<<wellbox->wellInfo();
    //LOG<<"camerainfo = "<<camerabox->cameraInfo();
}

PreviewToolInfo PreviewTool::toolInfo() const
{
    PreviewToolInfo info;

    auto wellinfo = wellbox->wellInfo();
    foreach(auto key,wellinfo.keys())
        info[key] = wellinfo[key];

    auto objectiveinfo = objectivebox->objectiveInfo();
    info[ObjectiveField] = objectiveinfo[ObjectiveField];

    auto channelinfo = channelbox->channelInfo();
    info[ChannelField] = channelinfo[ChannelField];

    auto camerainfo = camerabox->cameraInfo();

    QStringList channels;
    if (!camerainfo.isEmpty()) { // 有勾选的通道
        foreach(auto channel, camerainfo.keys()) {
            CameraInfo val = camerainfo[channel];// 每个通道的信息

            QVariantMap m;//把每个通道的信息转成QVarintMap
            m[ChannelField] = channel;
            m[ExposureField] = val[ExposureField];
            m[GainField] = val[GainField];
            m[BrightField] = val[BrightField];
            info[channel] = m; // 这个channel字段存储这个CameraInfo
            channels << channel;
        }
    }

    info[ChannelField] = channels;//增加一个key=channel,和objective,well等是平级的,方便组装时判断
    //LOG<<"tool info = "<<info;
    return info;
}