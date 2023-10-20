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

class INTERFACE_IMEXPORT PreviewTool : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewTool(QWidget*parent= nullptr);
    PreviewToolInfo toolInfo() const;
private:
    WellBox * wellbox;
    CameraBox * camerabox;
    QVBoxLayout * lay;
signals:
    // wellbox的信息
    void wellbrandChanged(int option);
    void welldishChanged(int option);
    void wellflaskChanged(int option);
    void wellslideChanged(int option);
    void wellsizeChanged(int option);
    void objectiveChanged(int option);
    // 相机不同通道的信息
    void phExposureChanged(int ms); // 1ms-15s
    void phGainChanged(int percent); // 100%-5000%
    void phBrightChanged(int bright); // 0-100
    void gfpExposureChanged(int ms);
    void gfpGainChanged(int percent);
    void gfpBrightChanged(int bright);
    void rfpExposureChanged(int ms);
    void rfpGainChanged(int percent);
    void rfpBrightChanged(int bright);
    void dapiExposureChanged(int ms);
    void dapiGainChanged(int percent);
    void dapiBrightChanged(int bright);
    void channelChanged(QVector<bool> states);
    // 上述所有信号都会触发此信号
    void infoChanged();

};

#endif //EOSI_PREVIEWTOOL_H
