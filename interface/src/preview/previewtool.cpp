/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:53:52
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-09 11:58:08
 * @FilePath: \EOS\interface\src\preview\previewtool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewtool.h"

PreviewTool::PreviewTool(QWidget *parent) : QWidget(parent)
{
    historybox = new HistoryBox;
    objectivebox = new ObjectiveBox;
    selectbox = new ViewModeBox;
    channelbox = new ChannelBox;
    camerabox = new CameraBox;
    recordbox = new RecordBox;

    lay = new QVBoxLayout;
    lay->addWidget(historybox);
    lay->addWidget(objectivebox);
    lay->addWidget(selectbox);
    lay->addWidget(channelbox);
    lay->addWidget(camerabox);
    lay->addWidget(recordbox);
    setLayout(lay);

    // 1. 信号直连
    connect(objectivebox,&ObjectiveBox::objectiveChanged,this,&PreviewTool::objectiveChanged);
    connect(objectivebox,&ObjectiveBox::objectiveToggled,this,&PreviewTool::objectiveToggled);
    connect(camerabox,&CameraBox::cameraInfoChanged,this,&PreviewTool::cameraInfoChanged);
    connect(camerabox,&CameraBox::photoTaking,this,&PreviewTool::photoTaking);
    connect(camerabox,&CameraBox::slideStitching,this,&PreviewTool::slideStitching);
    connect(camerabox,&CameraBox::cameraAdjusted,this,&PreviewTool::cameraAdjusted);
    connect(camerabox,&CameraBox::brightAdjusted,this,&PreviewTool::brightAdjusted);
    connect(camerabox,&CameraBox::focusChanged,this,&PreviewTool::focusChanged);
    connect(channelbox,&ChannelBox::channelChanged,this,&PreviewTool::channelChanged);
    connect(channelbox,&ChannelBox::channelClosed,this,&PreviewTool::channelClosed);
    connect(historybox,&HistoryBox::importFilePath,this,&PreviewTool::importFilePath);
    connect(recordbox,&RecordBox::pauseVideo,this,&PreviewTool::pauseVideo);
    connect(recordbox,&RecordBox::playVideo,this,&PreviewTool::playVideo);
    connect(recordbox,&RecordBox::stopVideo,this,&PreviewTool::stopVideo);
    connect(selectbox,&ViewModeBox::modeSelected,this,&PreviewTool::modeSelected);
    connect(selectbox,&ViewModeBox::triangleClicked,this,&PreviewTool::triangleClicked);
    connect(selectbox,&ViewModeBox::enableWellPattern,this,&PreviewTool::enableWellPattern);
    connect(selectbox,&ViewModeBox::groupTypeChanged,this,&PreviewTool::groupTypeChanged);
    connect(selectbox,&ViewModeBox::groupColorChanged,this,&PreviewTool::groupColorChanged);
    // 2. 信号槽函数
    connect(channelbox,&ChannelBox::channelChanged,camerabox,&CameraBox::updateChannelText);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,channelbox,&ChannelBox::disableChannel);
    // 3.外部信号
    connect(this,&PreviewTool::objectiveSettingChanged,objectivebox,&ObjectiveBox::onObjectiveSettingChanged);
    connect(this,&PreviewTool::captureImage,camerabox,&CameraBox::captureImage); // 保存通道的图像
    connect(this,&PreviewTool::captureImage,channelbox,&ChannelBox::takePhoto); // 显示缩略图
    connect(this,&PreviewTool::imageCaptured,recordbox,&RecordBox::recordImage); // 原始相机图像
    connect(this,&PreviewTool::exposureGainCaptured,camerabox,&CameraBox::captureExposureGain);
    recordbox->hide();
}

QMap<QString,QString> PreviewTool::boxInfo(const QString &box) const
{
    QMap<QString,QString> info;

    if (box == ObjectiveBoxTitle) {
        return objectivebox->objectiveInfo();
    } else if (box == ChannelBoxTitle) {
        return channelbox->channelInfo();
    } else if (box == CameraFocusBoxTitle) {
        return camerabox->cameraInfo(); // 注意: 这里只提供当前Ui的信息而不是5个通道的
    } else if (box == RecordBoxTitle) {
        return recordbox->recordInfo();
    } else if (box == ViewSelectBoxTitle) {
        return selectbox->viewModeInfo();
    }

    return info;
}

PreviewToolInfo PreviewTool::toolInfo() const
{
    PreviewToolInfo info;

    // 1. wellbox的brand+manufacturer
    //auto wellinfo = wellbox->wellInfo();
    //info[WellsizeField] = wellinfo[WellsizeField];
    //info[BrandField] = wellinfo[BrandField];
    //info[ManufacturerField] = wellinfo[ManufacturerField];
//    foreach(auto key,wellinfo.keys())
//        info[key] = wellinfo[key];

    // 2. objective
    auto objectiveinfo = objectivebox->objectiveInfo();
    info[ObjectiveLocationField] = objectiveinfo[ObjectiveLocationField];// 用了哪个位置的镜头
    info[ObjectiveField] = objectiveinfo[ObjectiveField]; // 4x
    info[ObjectiveDescripField] = objectiveinfo[ObjectiveDescripField]; // br4x
    info[ObjectiveTypeField] = objectiveinfo[ObjectiveTypeField];

    // 3. channel
    auto channelinfo = channelbox->channelInfo();//这个属于临时信息,其实没用(拍照这里有用,当前拍的通道)
    info[CurrentChannelField] = channelinfo[CurrentChannelField]; // 实际字符串BR,PH,如果都没开灯是空字符串

    // 4. camerainfo,分不同通道,保存了gain,exposure,bright
    auto camerainfo = camerabox->multiCameraInfo();
    info[CurrentInfoField].setValue(camerabox->cameraInfo());
    info[FocusField] = camerabox->focusValue();
    info[FocusStepField] = camerabox->focusStep();

    QStringList channels;//所有保存过相机配置的通道
    if (!camerainfo.isEmpty()) { // 有保存过的通道参数
        for(auto channel: camerainfo.keys()) { // channel=PH,BR
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

    // 8. viewmode
    auto viewmodeinfo = selectbox->viewModeInfo();
    info[ViewModeField] = viewmodeinfo[ViewModeField];

    //LOG<<"tool info = "<<info;
    return info;
}

void PreviewTool::importExperConfig(const QVariantMap &m)
{
    { // 1.wellbox
        //auto brand = m[BrandField].toUInt();
        //auto manufacturer = m[ManufacturerField].toUInt();
        //wellbox->importExperConfig(manufacturer,brand);
    }

    // 2. objectivebox
    auto objective = m[ObjectiveDescripField].toString(); // br4x
    objectivebox->importExperConfig(objective);
    // camera_loc自动跟随objective的设置,无需导入


    { // 6. 相机通道参数的导入
        auto camerainfo = m[CameraChannelField].value<QVariantMap>();
        if (!camerainfo.isEmpty()) { // 可能没保存过参数
            camerabox->importExperConfig(camerainfo,objective);
        }
    }

}