/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-22 13:30:58
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-22 13:36:00
 * @FilePath: \EOS\component\src\tool\recordimage.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "recordimage.h"

RecordImage::RecordImage(QObject *parent) : QObject(parent)
{
    imgdir = getAppStandardDirPath(); // 默认app/eos文件夹
}

void RecordImage::setImageDir(const QString &dir)
{
    imgdir = dir;
}

void RecordImage::setImageFormat(const QString &format)
{ // format是小写
    imgformat = "."+format.toLower();
}

void RecordImage::record()
{
    isRecording = true;
}

void RecordImage::stop()
{
    isRecording = false;
}

void RecordImage::setImage(const QImage &img)
{ // 点了录制就会一直存图
    if (!isRecording) return;

    auto path = imgdir+"/"+QDateTime::currentDateTime().toString(DefaultImageSaveDateTimeFormat)+imgformat;
    img.save(path);
}