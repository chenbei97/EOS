/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-18 09:07:05
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-18 13:28:37
 * @FilePath: \EOS\component\src\datastructure\imagetransformthread.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "imagetransformthread.h"

void ImageTransformThread::run()
{
    while(enable)
    {
        QMutexLocker locker(&mutex);
        if (!mimage.isNull()) {
            //LOG<<"image transform's thread: "<<CURRENT_THREAD;
            //auto img = mimage.mirrored(true,false);
            //QMatrix matrix;
            //matrix.rotate(270.0);
            //img = img.transformed(matrix,Qt::FastTransformation);
            //mimage = img.scaled(msize,Qt::KeepAspectRatio,Qt::FastTransformation);
            mimage = mimage.scaled(msize,Qt::KeepAspectRatio,Qt::FastTransformation);
            emit imageTransformed(mimage);
        } else {

            emit imageTransformed(QImage());
        }
        msleep(DefaultThreadSleepMs);
    }
}

void ImageTransformThread::setImage(const QImage &img)
{
    QMutexLocker locker(&mutex);
    mimage = img;
}

void ImageTransformThread::setImageSize(const QSize &size)
{
    msize = size;
}

void ImageTransformThread::stopThread()
{
    enable = false;
    quit();
    wait();
}

void ImageTransformThread::startThread()
{
    enable = true;
    start();
}

ImageTransformThread& ImageTransformThread::instance()
{
    static ImageTransformThread instance;
    return instance;
}