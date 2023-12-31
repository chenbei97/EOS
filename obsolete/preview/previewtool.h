/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:54:01
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:54:39
 * @FilePath: \EOS\interface\include\window\previewtool.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PREVIEWTOOL_H
#define EOSI_PREVIEWTOOL_H

#include "window.h"
#include "camerabox.h"
#include "channelbox.h"
#include "objectivebox.h"
#include "zstackbox.h"
#include "lensbox.h"
#include "savebox.h"
#include "timebox.h"
#include "recordbox.h"
#include "viewmode.h"

class INTERFACE_IMEXPORT PreviewTool : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewTool(QWidget*parent= nullptr);
    PreviewToolInfo toolInfo() const;
    CameraInfo cameraInfo() const;
    ObjectiveInfo objectiveInfo() const;
    FocusInfo focusInfo() const;
    ChannelInfo channelInfo() const;
    ViewModeInfo viewModeInfo() const;
    int currentExposure() const;
    int currentGain() const;
    int currentBright() const;
    int currentObjective() const;
    QString currentObjectiveDescription() const;
    QString currentChannel() const;
    QString currentGroup() const;
    int currentViewMode() const;
    void openDefaultChannel();
    void importExperConfig(const QVariantMap& m);
private:
    ObjectiveBox * objectivebox;
    ViewModeBox * selectbox;
    ChannelBox * channelbox;
    CameraBox * camerabox;
    RecordBox * recordbox;
    QVBoxLayout * lay;
signals:
    void channelChanged(int option);// 通道切换
    void channelClosed(int option); // 通道关闭
    void cameraInfoChanged(const MultiCameraInfo&m);// 通道配置改变
    void playVideo(const QString& path); // 播放视频的功能
    void stopVideo();
    void pauseVideo();
    void photoTaking();// 只有camerabox的拍照会触发该信号
    void slideStitching(); // slide下允许拼图
    void focusChanged(double val);
    void autoFocus();
    void cameraAdjusted(int exposure,int gain); // 调整相机参数
    void brightAdjusted(int bright); // 调整bright
    void objectiveChanged(const QString& obj);
    void objectiveToggled(int objective,int objective_loc,int isPh);
    void triangleClicked(int option); // 移动镜头
    void enableWellPattern(bool enable);
    void groupTypeChanged(const QString& group);
    void groupColorChanged(const QColor& color);
    void modeSelected(int mode); // 视野选择模式
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
    void groupSetted(const QString& text);
    void groupChanged(const QSet<QString>& groups);
    void wellbrandChanged(int option);
signals:
    void captureImage(const QImage& img,const QString& channel);
    void exposureGainCaptured(unsigned exp,unsigned gain);
    void imageCaptured(const QImage& img);
};

#endif //EOSI_PREVIEWTOOL_H
