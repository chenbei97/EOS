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
    LOG<<"image transform's thread: "<<CURRENT_THREAD;
    while(enable)
    {
        QMutexLocker locker(&mutex);
        if (!mimage.isNull()) {
            switch (mirrorType) {
                case MirrorType::NoMirror:
                    //LOG<<"no mirror";
                    break;
                case MirrorType::HorMirror:
                    mimage = mimage.mirrored(true,false);
                    //LOG<<"hor mirror";
                    break;
                case MirrorType::VerMirror:
                    mimage = mimage.mirrored(false,true);
                    //LOG<<"ver mirror";
                    break;
                case MirrorType::AllMirror:
                    mimage = mimage.mirrored(true,true);
                    //LOG<<"all mirror";
                    break;
            }
            if (rotateAngle > 0.0 && rotateAngle < 360.0) {
                QTransform transform;
                transform.rotate(rotateAngle);
                mimage = mimage.transformed(transform,Qt::FastTransformation);
            }
            mimage = mimage.scaled(msize,Qt::KeepAspectRatio,Qt::FastTransformation);
            emit imageTransformed(mimage);
        } else {
            emit imageTransformed(QImage());
        }
        msleep(DefaultThreadSleepMs);
    }
}

void ImageTransformThread::setImage(const QImage &img,double angle,MirrorType type)
{
    QMutexLocker locker(&mutex);
    mimage = img;
    rotateAngle = angle;
    mirrorType = type;
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