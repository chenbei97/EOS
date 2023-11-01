/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 09:15:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 09:15:59
 * @FilePath: \EOS\interface\src\window\camerabox.cpp
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
    connect(capturebtn,&PushButton::clicked,this,&CameraBox::onCaptureBtn);
}

void CameraBox::onSaveBtn()
{ // 保存参数也不需要
    auto channel = currentchannel->text().remove(ChannelFieldLabel);
    camerainfo[channel] = saveInfo();
    emit cameraInfoChanged(camerainfo);
    //LOG<<"camera info = "<<camerainfo;
}

void CameraBox::onCaptureBtn()
{
    emit photoTaking();
}

void CameraBox::onCombineBtn()
{

}

void CameraBox::setEnabled(bool enabled)
{ // 除了br,phase,其它通道不需要调整相机参数
    cameratool->setEnabled(enabled);
    savebtn->setEnabled(enabled);
    currentchannel->setEnabled(enabled);
}

void CameraBox::setChannel(int option)
{
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
    } else {
        cameratool->setBright(0);
        cameratool->setExposure(0);
        cameratool->setGain(0);
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