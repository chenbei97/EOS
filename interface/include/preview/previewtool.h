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
//#include "channelbox.h"
#include "channelboxx.h"
#include "wellbox.h"
#include "objectivebox.h"
#include "zstackbox.h"
#include "lensbox.h"
#include "savebox.h"
#include "timebox.h"
#include "historybox.h"
#include "recordbox.h"
#include "viewmode.h"

class INTERFACE_IMEXPORT PreviewTool : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewTool(QWidget*parent= nullptr);
    PreviewToolInfo toolInfo() const;
    QMap<QString,QString> boxInfo(const QString& box) const;
    void importExperConfig(const QVariantMap& m);
private:
    HistoryBox * historybox;
    ViewModeBox * selectbox;
    WellBox * wellbox;
    ObjectiveBox * objectivebox;
    FocusBox * focusbox;
    ChannelBoxx * channelbox;
    CameraBox * camerabox;
    RecordBox * recordbox;
    TimeBox * timebox;
    ZStackBox * zstackbox;
    SaveBox * savebox;
    QVBoxLayout * lay;
signals:
    void welltypeChanged(int option);
    void wellbrandChanged(int option);// wellbox的信息
    void manufacturerChanged(int option);
    void objectiveChanged(const QString& obj);
    void objectiveToggled(int objective,int objective_loc,int isPh);
    void channelChanged(int option);// 通道切换
    void channelClosed(int option); // 通道关闭
    void cameraInfoChanged(const MultiCameraInfo&m);// 通道配置改变
    void zstackChanged(bool isChecked);// 是否勾选zstack和stitch
    void stitchChanged(bool isChecked);
    void playVideo(const QString& path); // 播放视频的功能
    void stopVideo();
    void pauseVideo();
    void exportFilePath(const QString&path); // 导出文件配置的路径
    void importFilePath(const QString&path); // 导入文件配置的路径
    void photoTaking();// 只有camerabox的拍照会触发该信号
    void slideStitching(); // slide下允许拼图
    void directionMove(int option); // 移动镜头
    void focusChanged(double val);
    void focusSliderPressed();
    void cameraAdjusted(int exposure,int gain); // 调整相机参数
    void brightAdjusted(int bright); // 调整bright
    void loadExper(); // 启动实验
    void stopExper(); // 停止实验
    void modeSelected(int mode); // 视野选择模式
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
    void captureImage(const QImage& img,const QString& channel);
    void exposureGainCaptured(unsigned exp,unsigned gain);
    void imageCaptured(const QImage& img);
};

#endif //EOSI_PREVIEWTOOL_H
