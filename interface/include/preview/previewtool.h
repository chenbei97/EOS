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
    ChannelBox * channelbox;
    CameraBox * camerabox;
    FocusBox * focusbox;
    LensBox * lensbox;
    TimeBox * timebox;
    ZStackBox * zstackbox;
    SaveBox * savebox;
    QVBoxLayout * lay;
signals:
    // wellbox的信息
    void wellbrandChanged(int option);
    void manufacturerChanged(int option);
    void objectiveChanged(int option);
    // 通道选择
    void channelChanged(int option);
    // 相机不同通道的信息
    void exposureChanged(int ms); // 1ms-15s
    void gainChanged(int percent); // 100%-5000%
    void brightChanged(int bright); // 0-100
    // 是否勾选zstack和stitch
    void zstackChanged(bool isChecked);
    void stitchChanged(bool isChecked);
    // 上述所有信号都会触发此信号
    void infoChanged();

};

#endif //EOSI_PREVIEWTOOL_H
