/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-15 14:28:53
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 17:05:22
 * @FilePath: \EOS\component\src\socket\requestqueuethread.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "requestqueuethread.h"

void RequestQueueThread::run()
{
    LOG<<"requestQueue's thread: "<<CURRENT_THREAD;
#ifdef useblockqueue
    while (true) {
        if (!requestQueue.isEmpty()) {
            auto request = requestQueue.dequeue();
            LOG<<"process request "<<request.second;
            emit requestDequeue(request);
        }
        msleep(DefaultThreadSleepMs);
    }
#else
    while (true) {
        QMutexLocker locker(&mutex);
        if (!requestQueue.isEmpty()) {
            condition.wait(&mutex);
            auto request = requestQueue.dequeue();
            LOG<<"process request "<<request.second;
            emit requestDequeue(request);
        }
        msleep(DefaultThreadSleepMs);
    }
#endif
}

void RequestQueueThread::enqueue(const QRequestMsg &msg)
{
#ifdef useblockqueue
    requestQueue.enqueue(msg);
#else
    QMutexLocker locker(&mutex);
    requestQueue.enqueue(msg);
    condition.wakeOne();
#endif
}

RequestQueueThread::RequestQueueThread(QObject *parent) : QThread(parent)
{

}