/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-18 09:06:41
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-18 13:28:44
 * @FilePath: \EOS\component\include\datastructure\imagetransformthread.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_IMAGETRANSFORMTHREAD_H
#define EOS_IMAGETRANSFORMTHREAD_H

#include "datastructure.h"
//extern const int DefaultThreadSleepMs;

class COMPONENT_IMEXPORT ImageTransformThread : public QThread
{
    Q_OBJECT
public:
    void setImage(const QImage& img);
    void setImageSize(const QSize& size);
    void startThread();
    void stopThread();
    static ImageTransformThread& instance();
private:
    void run() override;
    bool enable = false; // 默认不启用
    QImage mimage;
    QSize msize;
    QMutex mutex;
signals:
    void imageTransformed(const QImage& img);
};

#define ImageTransformThreadPointer (&ImageTransformThread::instance())
#endif //EOS_IMAGETRANSFORMTHREAD_H
