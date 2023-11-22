/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-22 13:30:44
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-22 13:35:54
 * @FilePath: \EOS\component\include\tool\recordimage.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_RECORDIMAGE_H
#define EOS_RECORDIMAGE_H

#include "tool.h"
#include "utilities.h"

class RecordImage: public QObject
{
    Q_OBJECT
public:
    explicit RecordImage(QObject*parent= nullptr);
    void setImageDir(const QString& dir);
    void setImageFormat(const QString& format = JPGField);
    void setImage(const QImage& img);
    void record();
    void stop();
private:
    QString imgdir;
    QString imgformat = JPGSuffix;
    QImage imgdata;
    bool isRecording = false;
};

#endif //EOS_RECORDIMAGE_H
