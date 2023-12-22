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
#include "focusbox.h"

#ifdef use_opencv2
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

static cv::Mat qimageToMat(const QImage& img)
{
    auto image = img.convertToFormat(QImage::Format_BGR888); // 只能是RGB888或者BGR888
    cv::Mat mat(image.height(), image.width(), // 只能转为CV_8UC3
                          CV_8UC3,(void*)image.constBits(),image.bytesPerLine());
    return mat.clone();// 需要深拷贝,临时变量会没
}

static QImage matToqimage(const cv::Mat& mat)
{
    QImage img((const unsigned char*)mat.data, mat.cols, mat.rows,
            mat.step,QImage::Format_BGR888);
//    auto lab = new Label;
//    lab->setPixmap(QPixmap::fromImage(img));
//    lab->show();
     return img;
}
#endif

class INTERFACE_IMEXPORT CameraBox : public GroupBox
{
    Q_OBJECT
public:
    explicit CameraBox(QWidget*parent= nullptr);
    void importExperConfig(const QVariantMap& m,const QString&objective);
    void captureImage(const QImage&img,const QString& channel);
    void captureExposureGain(unsigned exp,unsigned gain);
    void setEnabled(bool enabled);
    void setChannel(int option);
    MultiCameraInfo multiCameraInfo() const;
    CameraInfo cameraInfo() const;
private:
    CameraTool * cameratool;
    Label * currentchannel;
    PushButton * savebtn;
    PushButton * autofocusbtn;
    PushButton * savefocusbtn;
    PushButton * capturebtn;
    PushButton * stitchbtn;
    RoundButton * topbtn;
    RoundButton * bottombtn;
    DoubleSlider * slider;
    SpinBox * step;
    MultiCameraInfo camerainfo;
    ChannelImageInfo imageinfo;
    void initObjects();
    void initAttributes();
    void initLayout();
private slots:
    void onSaveBtn();
    void adjustBright();
    void adjustCamera();
signals:
    void cameraInfoChanged(const MultiCameraInfo &);
    void photoTaking();
    void slideStitching();
    void cameraAdjusted(int exposure,int gain);
    void brightAdjusted(int bright);
};
#endif //EOSI_CAMERABOX_H
