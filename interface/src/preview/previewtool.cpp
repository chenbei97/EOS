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
    selectbox = new ViewSelect;
    wellbox = new WellBox;
    objectivebox = new ObjectiveBox;
    focusbox = new FocusBox;
    channelbox = new ChannelBoxx;
    camerabox = new CameraBox;
    recordbox = new RecordBox;
    timebox = new TimeBox;
    zstackbox = new ZStackBox;
    savebox = new SaveBox;

    lay = new QVBoxLayout;
    lay->addWidget(historybox);
    lay->addWidget(wellbox);
    lay->addWidget(selectbox);
    lay->addWidget(objectivebox);
    lay->addWidget(focusbox);
    lay->addWidget(channelbox);
    lay->addWidget(camerabox);
    lay->addWidget(recordbox);
    lay->addWidget(timebox);
    lay->addWidget(zstackbox);
    lay->addWidget(savebox);
    setLayout(lay);

    // 1. 信号直连
    connect(wellbox,&WellBox::wellbrandChanged,this,&PreviewTool::wellbrandChanged);
    connect(wellbox,&WellBox::welltypeChanged,this,&PreviewTool::welltypeChanged);
    connect(wellbox,&WellBox::manufacturerChanged,this,&PreviewTool::manufacturerChanged);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,this,&PreviewTool::objectiveChanged);
    connect(objectivebox,&ObjectiveBox::objectiveToggled,this,&PreviewTool::objectiveToggled);
    connect(camerabox,&CameraBox::cameraInfoChanged,this,&PreviewTool::cameraInfoChanged);
    connect(camerabox,&CameraBox::photoTaking,this,&PreviewTool::photoTaking);
    connect(camerabox,&CameraBox::slideStitching,this,&PreviewTool::slideStitching);
    connect(camerabox,&CameraBox::cameraAdjusted,this,&PreviewTool::cameraAdjusted);
    connect(camerabox,&CameraBox::brightAdjusted,this,&PreviewTool::brightAdjusted);
    connect(channelbox,&ChannelBoxx::channelChanged,this,&PreviewTool::channelChanged);
    connect(zstackbox,&ZStackBox::zstackChanged,this,&PreviewTool::zstackChanged);
    connect(zstackbox,&ZStackBox::stitchChanged,this,&PreviewTool::stitchChanged);
    connect(historybox,&HistoryBox::importFilePath,this,&PreviewTool::importFilePath);
    connect(savebox,&SaveBox::exportFilePath,this,&PreviewTool::exportFilePath);
    connect(savebox,&SaveBox::loadExper,this,&PreviewTool::loadExper);
    connect(savebox,&SaveBox::stopExper,this,&PreviewTool::stopExper);
    connect(focusbox,&FocusBox::directionMove,this,&PreviewTool::directionMove);
    connect(focusbox,&FocusBox::sliderPressed,this,&PreviewTool::focusSliderPressed);
    connect(focusbox,&FocusBox::valueChanged,this,&PreviewTool::focusChanged);
    connect(recordbox,&RecordBox::pauseVideo,this,&PreviewTool::pauseVideo);
    connect(recordbox,&RecordBox::playVideo,this,&PreviewTool::playVideo);
    connect(recordbox,&RecordBox::stopVideo,this,&PreviewTool::stopVideo);
    connect(selectbox,&ViewSelect::modeSelected,this,&PreviewTool::modeSelected);
    // 2. 信号槽函数
    connect(channelbox,&ChannelBoxx::channelChanged,camerabox,&CameraBox::setChannel);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,channelbox,&ChannelBoxx::disableChannel);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,timebox,&TimeBox::disableChannel);
    connect(wellbox,&WellBox::welltypeChanged,selectbox,&ViewSelect::setEnabled);
    // 3.外部信号
    connect(this,&PreviewTool::objectiveSettingChanged,objectivebox,&ObjectiveBox::onObjectiveSettingChanged);
    connect(this,&PreviewTool::captureImage,camerabox,&CameraBox::captureImage); // 当前通道的图像
    connect(this,&PreviewTool::captureImage,channelbox,&ChannelBoxx::takePhoto);
    connect(this,&PreviewTool::imageCaptured,recordbox,&RecordBox::recordImage); // 原始相机图像
    connect(this,&PreviewTool::exposureGainCaptured,camerabox,&CameraBox::captureExposureGain);

#ifdef usetab
    savebox->hide(); // 懒得删代码直接隐藏
    zstackbox->hide();
    timebox->hide();
#endif
}

QMap<QString,QString> PreviewTool::boxInfo(const QString &box) const
{
    QMap<QString,QString> info;

    if (box == WellBoxTitle) {
        return wellbox->wellInfo();
    } else if (box == ObjectiveBoxTitle) {
        return objectivebox->objectiveInfo();
    } else if (box == ChannelBoxTitle) {
        return channelbox->channelInfo();
    } else if (box == CameraBoxTitle) {
        return camerabox->cameraInfo(); // 注意: 这里只提供当前Ui的信息而不是5个通道的
    } else if (box == FocusBoxTitle) {
        return focusbox->focusInfo();
    } else if (box == OtherBoxField) {
        return zstackbox->zstackInfo();
    } else if (box == RecordBoxTitle) {
        return recordbox->recordInfo();
    } else if (box == ExperimentBoxTitle) {
        return timebox->timeInfo();
    }

    return info;
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

void PreviewTool::importExperConfig(const QVariantMap &m)
{
    { // 1.wellbox
        auto brand = m[BrandField].toUInt();
        auto manufacturer = m[ManufacturerField].toUInt();
        wellbox->importExperConfig(manufacturer,brand);
    }

    // 2. objectivebox
    auto objective = m[ObjectiveDescripField].toString(); // br4x
    objectivebox->importExperConfig(objective);
    // camera_loc自动跟随objective的设置,无需导入

    { // 3. focusbox
        auto focus = m[FocusField].toDouble();
        auto step = m[FocusStepField].toDouble();
        focusbox->importExperConfig(focus,step);
    }

    { // 4.stackbox
        auto zstack = m[ZStackField].toBool();
        auto stitch = m[StitchField].toBool();
        zstackbox->importExperConfig(zstack,stitch);
    }

    { // 5. timebox
        auto is_schedule = m[IsScheduleField].toBool();
        auto total = m[TotalTimeField].toLongLong();
        auto duration = m[DurationTimeField].toLongLong();
        auto channels = m[ChannelField].toString().split(",",QString::SkipEmptyParts);
        timebox->importExperConfig(is_schedule,total,duration,channels,objective);
    }

    { // 6. 相机通道参数的导入
        auto camerainfo = m[CameraChannelField].value<QVariantMap>();
        if (!camerainfo.isEmpty()) { // 可能没保存过参数
            camerabox->importExperConfig(camerainfo,objective);
        }
    }

}