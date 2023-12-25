/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 09:15:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-02 17:31:56
 * @FilePath: \EOS\interface\include\preview\camerabox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_CAMERABOX_H
#define EOSI_CAMERABOX_H

#include "window.h"
#include "cameratool.h"

class INTERFACE_IMEXPORT CameraBox : public GroupBox
{
    Q_OBJECT
public:
    explicit CameraBox(QWidget*parent= nullptr);
    void importExperConfig(const QVariantMap& m,const QString&objective);
    void captureImage(const QImage&img,const QString& channel);
    void captureExposureGain(unsigned exp,unsigned gain);
    void setBrightEnabled(bool enabled);
    void updateChannelText(int option);
    MultiCameraInfo multiCameraInfo() const;
    CameraInfo cameraInfo() const;
    FocusInfo focusInfo() const;
    double focusValue() const;
    double focusStep() const;
private:
    GroupBox * leftbox;
    CameraTool * cameratool;
    Label * currentchannel;
    PushButton * savecamerabtn;
    PushButton * autofocusbtn;
    PushButton * savefocusbtn;
    PushButton * capturebtn;
    PushButton * stitchbtn;
    RoundButton * topbtn;
    RoundButton * bottombtn;
    DoubleSlider * focusslider;
    SpinBox * focusstep;
    MultiCameraInfo camerainfo;
    ChannelImageInfo imageinfo;
    void initObjects();
    void initAttributes();
    void initLayout();
private slots:
    void addFocus();
    void subtractFocus();
    void saveCamera();
    void adjustBright();
    void adjustCamera();
signals:
    void cameraInfoChanged(const MultiCameraInfo &);
    void photoTaking();
    void slideStitching();
    void cameraAdjusted(int exposure,int gain);
    void brightAdjusted(int bright);
    void focusChanged(double val);
};
#endif //EOSI_CAMERABOX_H
