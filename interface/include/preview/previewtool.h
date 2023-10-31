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
#include "wellbox.h"
#include "objectivebox.h"
#include "zstackbox.h"
#include "lensbox.h"
#include "savebox.h"
#include "timebox.h"
#include "historybox.h"

class INTERFACE_IMEXPORT PreviewTool : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewTool(QWidget*parent= nullptr);
    PreviewToolInfo toolInfo() const;
private:
    HistoryBox * historybox;
    WellBox * wellbox;
    ObjectiveBox * objectivebox;
    FocusBox * focusbox;
    ChannelBox * channelbox;
    CameraBox * camerabox;
    LensBox * lensbox;
    TimeBox * timebox;
    ZStackBox * zstackbox;
    SaveBox * savebox;
    QVBoxLayout * lay;
signals:
    void wellbrandChanged(int option);// wellbox的信息
    void manufacturerChanged(int option);
    void objectiveChanged(int option);
    void channelChanged(int option);// 通道选择
    void cameraInfoChanged(const MultiCameraInfo&m);// 通道配置改变
    void zstackChanged(bool isChecked);// 是否勾选zstack和stitch
    void stitchChanged(bool isChecked);
    void photoTaking();// 只有camerabox的拍照会触发该信号
signals:
    void objectiveSettingChanged(int option);
};

#endif //EOSI_PREVIEWTOOL_H
