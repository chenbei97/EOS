/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:53:52
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-26 13:36:44
 * @FilePath: \EOS\interface\src\preview\previewtool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewtool.h"

PreviewTool::PreviewTool(QWidget *parent) : QWidget(parent)
{
    objectivebox = new ObjectiveBox;
    selectbox = new ViewModeBox;
    channelbox = new ChannelBox;
    camerabox = new CameraBox;
    recordbox = new RecordBox;

    lay = new QVBoxLayout;
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
    connect(this,&PreviewTool::groupSetted,selectbox,&ViewModeBox::updateGroupItemIcon);
    connect(this,&PreviewTool::groupChanged,selectbox,&ViewModeBox::resetGroupItemIcon);
    connect(this,&PreviewTool::wellbrandChanged,selectbox,&ViewModeBox::setViewEnabled);
    recordbox->hide();
}

PreviewToolInfo PreviewTool::toolInfo() const
{
    PreviewToolInfo info;

    // 1. objective
    auto objectiveinfo = objectivebox->objectiveInfo();
    info[ObjectiveLocationField] = objectiveinfo[ObjectiveLocationField];// 用了哪个位置的镜头
    info[ObjectiveField] = objectiveinfo[ObjectiveField]; // 4x
    info[ObjectiveDescripField] = objectiveinfo[ObjectiveDescripField]; // br4x
    info[ObjectiveTypeField] = objectiveinfo[ObjectiveTypeField];

    // 2. channel
    auto channelinfo = channelbox->channelInfo();//这个属于临时信息,其实没用(拍照这里有用,当前拍的通道)
    info[CurrentChannelField] = channelinfo[CurrentChannelField]; // 实际字符串BR,PH,如果都没开灯是空字符串

    // 3. camerainfo,分不同通道,保存了gain,exposure,bright
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

    // 4. viewmode
    auto viewmodeinfo = selectbox->viewModeInfo();
    info[ViewModeField] = viewmodeinfo[ViewModeField];
    info[CurrentGroupField] = viewmodeinfo[CurrentGroupField];

    //LOG<<"tool info = "<<info;
    return info;
}

void PreviewTool::importExperConfig(const QVariantMap &m)
{
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

ObjectiveInfo PreviewTool::objectiveInfo() const
{
    return objectivebox->objectiveInfo();
}

int PreviewTool::currentObjective() const
{ // <=> getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    return getIndexFromFields(objectivebox->objectiveInfo()[ObjectiveField]).toUInt();
}

QString PreviewTool::currentObjectiveDescription() const
{ // <=> toolinfo[ObjectiveDescripField].toString()
    return objectivebox->objectiveInfo()[ObjectiveDescripField];
}

CameraInfo PreviewTool::cameraInfo() const
{ // 注意是当前ui的 <=> toolinfo[CurrentInfoField].value<CameraInfo>()
    return camerabox->cameraInfo();
}

FocusInfo PreviewTool::focusInfo() const
{
    return camerabox->focusInfo();
}

ChannelInfo PreviewTool::channelInfo() const
{
    return channelbox->channelInfo();
}

QString PreviewTool::currentChannel() const
{ // 一个快捷方法 <=> toolinfo[CurrentChannelField].toString()
    return channelInfo()[CurrentChannelField];
}

void PreviewTool::openDefaultChannel()
{// 检测到没开任何灯时要手动先打开默认(明场)通道
    channelbox->openDefaultChannel();
}

int PreviewTool::currentExposure() const
{// 快捷方法,当前Ui显示的值
    return camerabox->exposure();
}

int PreviewTool::currentBright() const
{// 快捷方法,当前Ui显示的值
    return camerabox->bright();
}

int PreviewTool::currentGain() const
{ // 快捷方法,当前Ui显示的值
    return camerabox->gain();
}

ViewModeInfo PreviewTool::viewModeInfo() const
{
    return selectbox->viewModeInfo();
}

QString PreviewTool::currentGroup() const
{
    return selectbox->currentGroup();
}

int PreviewTool::currentViewMode() const
{
    return (int)selectbox->viewMode();
}