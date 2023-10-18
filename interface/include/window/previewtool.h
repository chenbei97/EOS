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

class INTERFACE_IMEXPORT PreviewTool : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewTool(QWidget*parent= nullptr);
private:
    ComboBox * wellbrandCombo;
    ComboBox * wellsizeCombo;
    ComboBox * welldishCombo;
    ComboBox * wellflaskCombo;
    ComboBox * wellslideCombo;
    ComboBox * objectiveCombo;
    GroupBox* initWellBox();

    Slider * cameraExposureSlider;
    Slider * cameraGainSlider;
    Slider * cameraBrightSlider;
    GroupBox* initCameraBox();

    QVBoxLayout * lay;
signals:
    void wellbrandChanged(int option);
    void welldishChanged(int option);
    void wellflaskChanged(int option);
    void wellslideChanged(int option);
    void wellsizeChanged(int option);
    void objectiveChanged(int option);
    void channelChanged(const QList<int>& options);
    void exposureChanged(int ms); // 1ms-15s
    void gainChanged(int percent); // 100%-5000%
    void brightChanged(int bright); // 0-100

};

#endif //EOSI_PREVIEWTOOL_H
