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
    initObjects();
    initAttributes();
    initLayout();
    setTitle(tr(CameraBoxTitle));

    connect(savebtn,&PushButton::clicked,this,&CameraBox::onSaveBtn);
    connect(capturebtn,&PushButton::clicked,this,&CameraBox::photoTaking);
    connect(stitchbtn,&PushButton::clicked,this,&CameraBox::slideStitching);
    connect(cameratool,&CameraTool::exposureChanged,this,&CameraBox::adjustCamera);
    connect(cameratool,&CameraTool::gainChanged,this,&CameraBox::adjustCamera);
    connect(cameratool,&CameraTool::brightChanged,this,&CameraBox::adjustBright);
}

void CameraBox::initLayout()
{
    auto box1 = new GroupBox;
    auto box1lay = new QVBoxLayout;
    box1lay->addWidget(cameratool);
    box1lay->addWidget(savebtn);
    box1->setLayout(box1lay);

    auto box2 = new GroupBox;
    auto box2lay = new QHBoxLayout;
    auto box2lay_1 = new QVBoxLayout; // box2的左半部分
    box2lay_1->addWidget(autofocusbtn);
    box2lay_1->addWidget(savefocusbtn);
    box2lay_1->addWidget(capturebtn);
    box2lay_1->addWidget(stitchbtn);
    auto box2lay_2 = new QHBoxLayout; // box2的右半部分
    box2lay_2->addWidget(slider);
    auto box2lay_2_1 = new QVBoxLayout; // 2个三角按钮是垂直布局
    box2lay_2_1->addWidget(topbtn);
    box2lay_2_1->addWidget(bottombtn);
    box2lay_2->addLayout(box2lay_2_1);
    box2lay->addLayout(box2lay_1);
    box2lay->addLayout(box2lay_2);
    box2->setLayout(box2lay);

    auto hlay = new QHBoxLayout;
    hlay->addWidget(box1);
    hlay->addWidget(box2);
    hlay->addStretch();
    auto lay = new QVBoxLayout;
    lay->addWidget(currentchannel);
    lay->addLayout(hlay);

    setLayout(lay);
}

void CameraBox::initObjects()
{
    cameratool = new CameraTool;
    currentchannel = new Label(QString("%1%2").arg(ChannelFieldLabel).arg(BRField));
    savebtn = new PushButton(tr(SaveCurrentChannelSettingLabelField));
    autofocusbtn = new PushButton(tr(AutoFocusField));
    savefocusbtn = new PushButton(tr(SaveFocusField));
    capturebtn = new PushButton(tr(CaptureField));
    stitchbtn = new PushButton(tr(StitchField));
    topbtn = new RoundButton;
    bottombtn = new RoundButton;
    slider = new DoubleSlider;
    step = new SpinBox(true);
}

void CameraBox::initAttributes()
{
    setEnabled(false); // 初始没开灯不能使用
    capturebtn->setBackGroundColor(Qt::yellow);

    slider->setDirection(Qt::Vertical);
    slider->setRange(0,FocusToolFocusMaxVal);
    slider->setScaleFactor(FocusToolScaleVal);
    slider->setMouseEvent(true);
    slider->setPrefixVisible(false);

    step->setValue(0.000);
    step->setMaximumWidth(FocusToolStepSpinMaxWidth);
    step->setSingleStep(FocusToolStepVal);
    step->setDecimals(FocusToolStepDecimal);
    step->setMaximum(FocusToolStepMaxVal);

    topbtn->setStrategy(RoundButton::TopTriangle);
    bottombtn->setStrategy(RoundButton::BottomTriangle);
}

void CameraBox::importExperConfig(const QVariantMap &m,const QString& objective)
{
    for(auto channel: m.keys()) {
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

    setEnabled(false); // 导入配置时默认都不可滑动,上述代码之后
    emit cameraInfoChanged(camerainfo);
}

void CameraBox::adjustCamera()
{ // cameratool's ui调节时更改相机的2个参数
    auto exp = cameratool->exposure().toUInt();
    auto gain = cameratool->gain().toUInt();
    emit cameraAdjusted(exp,gain);
}

void CameraBox::adjustBright()
{ // cameratool's ui调节时更改bright
    auto br = cameratool->bright().toUInt();
    emit brightAdjusted(br);
}

void CameraBox::onSaveBtn()
{ // 保存通道参数
    auto channel = currentchannel->text().remove(ChannelFieldLabel);
    if (channel == NoneField) return;

    camerainfo[channel] = cameraInfo();
    emit cameraInfoChanged(camerainfo);
}

void CameraBox::captureImage(const QImage &img, const QString &channel)
{ // 拍照返回来的图像和通道
    //LOG<<channel;
    if (channel.isEmpty()) return; // 都没开灯拍的黑图不存储

    imageinfo[channel] = img;
    LOG<<imageinfo;
}

void CameraBox::captureExposureGain(unsigned int exp, unsigned int gain)
{
    //LOG<<"回显值: "<<exp<<gain;
    const QSignalBlocker blocker(cameratool);
    cameratool->setExposure(exp);
    cameratool->setGain(gain);
}

void CameraBox::setEnabled(bool enabled)
{ // 除了br,phase,其它通道不需要调整相机参数
    cameratool->setEnabled(2,enabled); // 只需要bright滑动条禁用
    savebtn->setEnabled(enabled);
    currentchannel->setEnabled(enabled);
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

MultiCameraInfo CameraBox::multiCameraInfo() const
{
    return camerainfo;
}

CameraInfo CameraBox::cameraInfo() const
{
    CameraInfo info;
    info[BrightField] = cameratool->bright();
    info[GainField] = cameratool->gain();
    info[ExposureField] = cameratool->exposure();
    return info;
}