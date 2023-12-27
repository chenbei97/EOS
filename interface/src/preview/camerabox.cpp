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
    setTitle(tr(CameraFocusBoxTitle));

    connect(ToupCameraPointer,&ToupCamera::defaultExposureGain,this,&CameraBox::initCamera);
    connect(savecamerabtn,&PushButton::clicked,this,&CameraBox::saveCamera);
    connect(capturebtn,&PushButton::clicked,this,&CameraBox::photoTaking);
    connect(stitchbtn,&PushButton::clicked,this,&CameraBox::slideStitching);
    connect(cameratool,&CameraTool::exposureChanged,this,&CameraBox::adjustCamera);
    connect(cameratool,&CameraTool::gainChanged,this,&CameraBox::adjustCamera);
    connect(cameratool,&CameraTool::brightChanged,this,&CameraBox::adjustBright);
    connect(topbtn,&PushButton::clicked,this,&CameraBox::addFocus);
    connect(bottombtn,&PushButton::clicked,this,&CameraBox::subtractFocus);
    connect(focusslider,&DoubleSlider::sliderReleased,this,&CameraBox::focusChanged);
    connect(autofocusbtn,&PushButton::clicked,this,&CameraBox::autoFocus);
    connect(savefocusbtn,&PushButton::clicked,this,&CameraBox::saveFocus);
}

void CameraBox::initCamera(unsigned exp,unsigned gain)
{ // 打开相机时有个默认的参数
    LOG<<"adjust exp gain slider init val"<<exp<<gain;
    cameratool->blockSignals(true);
    cameratool->setGain(gain);
    cameratool->setExposure(exp);
    cameratool->blockSignals(false);
}

void CameraBox::initLayout()
{
    auto box1lay = new QVBoxLayout;
    box1lay->addWidget(cameratool);
    box1lay->addWidget(savecamerabtn);
    box1lay->setMargin(0);
    box1lay->setSpacing(0);
    leftbox->setMinimumWidth(CameraBoxLeftBoxMinWidth);//让滑动条看起来不要太窄否则不会方便滑动
    leftbox->setLayout(box1lay);

    auto box2 = new GroupBox;
    auto box2lay = new QHBoxLayout;
    auto box2lay_1 = new QVBoxLayout; // box2的左半部分
    box2lay_1->addWidget(autofocusbtn);
    box2lay_1->addWidget(savefocusbtn);
    box2lay_1->addWidget(capturebtn);
    box2lay_1->addWidget(stitchbtn);
    auto box2lay_2 = new QHBoxLayout; // box2的右半部分
    box2lay_2->addWidget(focusslider);
    auto box2lay_2_1 = new QVBoxLayout; // 2个三角按钮是垂直布局
    box2lay_2_1->addWidget(topbtn);
    box2lay_2_1->addStretch();
    box2lay_2_1->addWidget(focusstep);
    box2lay_2_1->addStretch();
    box2lay_2_1->addWidget(bottombtn);
    box2lay_2->addLayout(box2lay_2_1);
    box2lay_2->addStretch();
    box2lay_2->setSpacing(0);
    box2lay->addLayout(box2lay_1);
    box2lay->addSpacing(20);
    box2lay->addLayout(box2lay_2);
    box2->setLayout(box2lay);

    auto hlay = new QHBoxLayout;
    hlay->addWidget(leftbox);
    hlay->addWidget(box2);
    //hlay->addStretch();
    auto lay = new QVBoxLayout;
    lay->addWidget(currentchannel);
    lay->addLayout(hlay);

    setLayout(lay);
}

void CameraBox::initObjects()
{
    cameratool = new CameraTool;
    leftbox = new GroupBox();
    currentchannel = new Label(QString("%1%2").arg(ChannelFieldLabel).arg(NoneField));
    savecamerabtn = new PushButton(tr(SaveCurrentChannelSettingLabelField));
    autofocusbtn = new PushButton(tr(AutoFocusField));
    savefocusbtn = new PushButton(tr(SaveFocusField));
    capturebtn = new PushButton(tr(CaptureField));
    stitchbtn = new PushButton(tr(StitchField));
    topbtn = new RoundButton;
    bottombtn = new RoundButton;
    focusslider = new DoubleSlider;
    focusstep = new SpinBox(true);
}

void CameraBox::initAttributes()
{
    setBrightEnabled(false); // 初始没开灯不能使用
    capturebtn->setBackGroundColor(Qt::yellow);
    leftbox->setTitle(QString("%1%2").arg(ChannelFieldLabel).arg(NoneField));
    currentchannel->hide();

    focusslider->setDirection(Qt::Vertical);
    focusslider->setRange(0,FocusToolFocusMaxVal);
    focusslider->setScaleFactor(FocusToolScaleVal);
    focusslider->setMouseEvent(true);
    focusslider->setPrefixVisible(false);

    focusstep->setValue(0.000);
    focusstep->setMaximumWidth(FocusToolStepSpinMaxWidth);
    focusstep->setSingleStep(FocusToolStepVal);
    focusstep->setDecimals(FocusToolStepDecimal);
    focusstep->setMaximum(FocusToolStepMaxVal);

    topbtn->setStrategy(RoundButton::TopTriangle);
    bottombtn->setStrategy(RoundButton::BottomTriangle);
    topbtn->setSize(30);
    bottombtn->setSize(30);
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
        updateChannelText(0); // 切到br通道更新ui设置
    }
    else if (objective.contains(PHField,Qt::CaseInsensitive)){
        camerainfo[BRField].clear();
        updateChannelText(1); // 切到ph通道更新ui设置
    }

    setBrightEnabled(false); // 导入配置时默认都不可滑动,上述代码之后
    emit cameraInfoChanged(camerainfo);
}

void CameraBox::autoFocus()
{
    emit autoFocusing();
}

void CameraBox::saveFocus()
{

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

void CameraBox::saveCamera()
{ // 保存通道参数
    //auto channel = currentchannel->text().remove(ChannelFieldLabel);
    auto channel = leftbox->title().remove(ChannelFieldLabel);
    if (channel == NoneField) return;

    camerainfo[channel] = cameraInfo();
    emit cameraInfoChanged(camerainfo);
}

void CameraBox::addFocus()
{
    auto val = focusstep->value();
    if (val == 0.0) return;
    focusslider->addValue(val);
}

void CameraBox::subtractFocus()
{
    auto val = focusstep->value();
    if (val == 0.0) return;
    focusslider->subtractValue(val);
}

void CameraBox::captureImage(const QImage &img, const QString &channel)
{ // 拍照返回来的图像和通道
    //LOG<<channel;
    if (channel.isEmpty()) return; // 都没开灯拍的黑图不存储
    imageinfo[channel] = img;
    //LOG<<imageinfo;
}

void CameraBox::captureExposureGain(unsigned int exp, unsigned int gain)
{
    //LOG<<"回显值: "<<exp<<gain;
    const QSignalBlocker blocker(cameratool);
    cameratool->setExposure(exp);
    cameratool->setGain(gain);
}

void CameraBox::setBrightEnabled(bool enabled)
{ // 除了br,phase,其它通道不需要调整相机参数
    cameratool->setEnabled(2,enabled); // 只需要bright滑动条禁用
    savecamerabtn->setEnabled(enabled);
    currentchannel->setEnabled(enabled);
}

void CameraBox::updateChannelText(int option)
{ // channelbox切换通道时要更新当前的通道
    if (option < 0) {
        currentchannel->setText(QString("%1%2").arg(ChannelFieldLabel).arg(NoneField));
        leftbox->setTitle(QString("%1%2").arg(ChannelFieldLabel).arg(NoneField));
        setBrightEnabled(false);
        return;
    }
    setBrightEnabled(true);
    // 1. channelbox设置通道后更新相机设置显示的当前通道信息
    auto channel = ChannelFields[option];
    currentchannel->setText(QString("%1%2").arg(ChannelFieldLabel).arg(channel));
    leftbox->setTitle(QString("%1%2").arg(ChannelFieldLabel).arg(channel));

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

CameraFocusInfo CameraBox::cameraFocusInfo() const
{
    CameraFocusInfo m;

    // ui上的参数
    m[CurrentExposureField].setValue(exposure());
    m[CurrentGainField].setValue(bright());
    m[CurrentBrightField].setValue(bright());
    m[FocusField] = focusValue();
    m[FocusStepField] = focusStep();

    // 保存的每个通道参数
    QStringList save_channels;
    if (!camerainfo.isEmpty()) {
        for(auto channel: camerainfo.keys()) { // channel=PH,BR
            CameraInfo val = camerainfo[channel];// 每个通道的信息

            QVariantMap tmp;//把每个通道的信息转成QVarintMap
            tmp[ExposureField] = val[ExposureField];
            tmp[GainField] = val[GainField];
            tmp[BrightField] = val[BrightField];
            m[channel] = tmp;
            save_channels << channel;
        }
    }
    m[SaveChannelsField] = save_channels;
    return m;
}

CameraInfo CameraBox::cameraInfo() const
{ // 当前UI的
    CameraInfo info;
    info[BrightField] = cameratool->bright();
    info[GainField] = cameratool->gain();
    info[ExposureField] = cameratool->exposure();
    return info;
}

int CameraBox::exposure() const
{
    return cameratool->exposure().toUInt();
}

int CameraBox::gain() const
{
    return cameratool->gain().toUInt();
}

int CameraBox::bright() const
{
    return cameratool->bright().toUInt();
}

double CameraBox::focusStep() const
{
    return focusstep->value();
}

double CameraBox::focusValue() const
{
    return focusslider->value();
}