/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 09:15:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-02 17:36:07
 * @FilePath: \EOS\interface\src\preview\camerabox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "camerabox.h"

CameraBox::CameraBox(QWidget*parent): GroupBox(parent)
{
    cameratool = new CameraTool;
    currentchannel = new Label(QString("%1%2").arg(ChannelFieldLabel).arg(BRField));
    savebtn = new PushButton(tr("保存当前通道设置"));
    combinebtn = new PushButton(tr("组合通道"));
    capturebtn = new PushButton(tr("拍照"));
    setEnabled(false); // 初始没开灯不能使用

    auto blay = new QHBoxLayout;
    blay->addStretch();
    blay->addWidget(currentchannel);
    blay->addWidget(capturebtn);
    blay->addWidget(combinebtn);
    blay->addWidget(savebtn);

    auto lay = new QVBoxLayout;
    lay->addWidget(cameratool);
    lay->addLayout(blay);

    setLayout(lay);
    setTitle(tr("亮度"));

    connect(savebtn,&PushButton::clicked,this,&CameraBox::onSaveBtn);
    connect(combinebtn,&PushButton::clicked,this,&CameraBox::onCombineBtn);
    connect(capturebtn,&PushButton::clicked,this,&CameraBox::photoTaking);
    connect(cameratool,&CameraTool::exposureChanged,this,&CameraBox::adjustCamera);
    connect(cameratool,&CameraTool::gainChanged,this,&CameraBox::adjustCamera);
    connect(cameratool,&CameraTool::brightChanged,this,&CameraBox::adjustCamera);
}

void CameraBox::importExperConfig(const QVariantMap &m,const QString& objective)
{
    foreach(auto channel,m.keys()) {
        auto config = m[channel].value<QVariantMap>();
        CameraInfo info;
        info[ExposureField] = config[ExposureField].toString();
        info[GainField] = config[GainField].toString();
        info[BrightField] = config[BrightField].toString();
        camerainfo[channel] = info;
    }

    // br和phase的通道参数应当根据物镜的硬件参数清除
    if (objective.contains(BRField,Qt::CaseInsensitive)) {
        camerainfo[PHField].clear();
        setChannel(0); // 切到br通道更新ui设置
    }
    else if (objective.contains(PHField,Qt::CaseInsensitive)){
        camerainfo[BRField].clear();
        setChannel(1); // 切到ph通道更新ui设置
    }

    emit cameraInfoChanged(camerainfo);
}

void CameraBox::adjustCamera()
{ // cameratool's ui调节时更改相机的2个参数和bright
    auto exp = cameratool->exposure().toUInt();
    auto gain = cameratool->gain().toUInt();
    auto br = cameratool->bright().toUInt();
    emit cameraAdjusted(exp,gain,br);
}

void CameraBox::onSaveBtn()
{ // 保存通道参数
    auto channel = currentchannel->text().remove(ChannelFieldLabel);
    if (channel == NoneField) return;

    camerainfo[channel] = saveInfo();
    emit cameraInfoChanged(camerainfo);
}

void CameraBox::onCombineBtn()
{

}

void CameraBox::captureImage(const QImage &img, const QString &channel)
{ // 拍照返回来的图像和通道
    //LOG<<channel;
    if (channel.isEmpty()) return; // 都没开灯拍的黑图不存储

    imageinfo[channel] = img;
    LOG<<imageinfo;
}

void CameraBox::setEnabled(bool enabled)
{ // 除了br,phase,其它通道不需要调整相机参数
    cameratool->setEnabled(enabled);
    savebtn->setEnabled(enabled);
    currentchannel->setEnabled(enabled);
    capturebtn->setEnabled(enabled);
}

void CameraBox::setChannel(int option)
{ // channelbox切换通道时要更新当前的通道
    if (option < 0) {
        currentchannel->setText(QString("%1%2").arg(ChannelFieldLabel).arg(NoneField));
        setEnabled(false);
        return;
    }
    setEnabled(true);
    // 1. channelbox设置通道后更新相机设置显示的当前通道信息
    auto channel = ChannelFields[option];
    currentchannel->setText(QString("%1%2").arg(ChannelFieldLabel).arg(channel));

    // 2. 防止3个滑动条设置信息的时候触发信号
    cameratool->blockSignals(true);

    // 3.切换通道时,新切换的通道如果设置过信息就同步到界面上
    if (camerainfo.keys().contains(channel)) { // 切换通道,如果通道有保存过的值,同步UI
        cameratool->setBright(camerainfo[channel][BrightField].toUInt());
        cameratool->setExposure(camerainfo[channel][ExposureField].toUInt());
        cameratool->setGain(camerainfo[channel][GainField].toUInt());
    } else { // 这里不要重置ui的数字,方便操作
//        cameratool->setBright(0);
//        cameratool->setExposure(0);
//        cameratool->setGain(0);
    }
    cameratool->blockSignals(false);
}

MultiCameraInfo CameraBox::cameraInfo() const
{
    return camerainfo;
}

CameraInfo CameraBox::saveInfo() const
{
    CameraInfo info;
    info[BrightField] = cameratool->bright();
    info[GainField] = cameratool->gain();
    info[ExposureField] = cameratool->exposure();
    return info;
}