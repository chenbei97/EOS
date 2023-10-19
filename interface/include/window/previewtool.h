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
private:
    WellBox * wellbox;
    CameraBox * camerabox;
    QVBoxLayout * lay;
signals:
    void wellbrandChanged(int option);
    void welldishChanged(int option);
    void wellflaskChanged(int option);
    void wellslideChanged(int option);
    void wellsizeChanged(int option);
    void objectiveChanged(int option);
};

#endif //EOSI_PREVIEWTOOL_H
