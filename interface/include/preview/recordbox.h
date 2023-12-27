/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-22 09:46:04
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-22 11:37:58
 * @FilePath: \EOS\interface\include\preview\recordbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_RECORDBOX_H
#define EOS_RECORDBOX_H

#include "window.h"

class INTERFACE_IMEXPORT RecordBox: public GroupBox
{
    Q_OBJECT
public:
    explicit RecordBox(QWidget*parent= nullptr);
    void recordImage(const QImage& img);
    RecordInfo recordInfo() const;
private:
    PushButton * startrecord;
    PushButton * stoprecord;
    Label * savepath;
    PushButton * playvideo;
    PushButton * stopvideo;
    PushButton * pausevideo;
    Label * selectvideo;
    ComboBox * videoformat;
    ComboBox * imageformat;
    ComboBox * framerate;
    RecordImage * recorder;
private:
    void changeSavePath();
    void selectVideo();
    void startRecord();
    void stopRecord();
signals:
    void playVideo(const QString& path);
    void stopVideo();
    void pauseVideo();
};

#endif //EOS_RECORDBOX_H
