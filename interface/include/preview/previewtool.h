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

class INTERFACE_IMEXPORT PreviewTool : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewTool(QWidget*parent= nullptr);
    PreviewToolInfo toolInfo() const;
    void importExperConfig(const QVariantMap& m);
private:
    HistoryBox * historybox;
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
    void wellbrandChanged(int option);// wellbox的信息
    void manufacturerChanged(int option);
    void objectiveChanged(const QString& obj);
    void channelChanged(int option);// 通道选择
    void cameraInfoChanged(const MultiCameraInfo&m);// 通道配置改变
    void zstackChanged(bool isChecked);// 是否勾选zstack和stitch
    void stitchChanged(bool isChecked);
    void playVideo(const QString& path); // 播放视频的功能
    void stopVideo();
    void pauseVideo();
    void exportFilePath(const QString&path); // 导出文件配置的路径
    void importFilePath(const QString&path); // 导入文件配置的路径
    void photoTaking();// 只有camerabox的拍照会触发该信号
    void directionMove(int option); // 移动镜头
    void cameraAdjusted(int exp,int gain,int br);// 调整相机实时更改
    void loadExper(); // 启动实验
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
    void captureImage(const QImage& img,const QString& channel);
    void exposureGainCaptured(unsigned exp,unsigned gain);
    void imageCaptured(const QImage& img);
};

#endif //EOSI_PREVIEWTOOL_H
