/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-15 14:27:56
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 17:05:29
 * @FilePath: \EOS\component\include\socket\requestqueuethread.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_REQUESTQUEUET_HREAD_H
#define EOS_REQUESTQUEUET_HREAD_H

#include "socket.h"
#include "qreadwritelock.h"
#include "qwaitcondition.h"

#define useblockqueue 0
#ifdef useblockqueue
#include "blockingqueue.h"
#endif

class COMPONENT_IMEXPORT RequestQueueThread: public QThread
{
    Q_OBJECT
public:
    explicit RequestQueueThread(QObject*parent= nullptr);
    void enqueue(const QRequestMsg& msg);
    void run() override;
private:

#ifdef useblockqueue
    BlockingQueue<QRequestMsg> requestQueue;
#else
    QRequestQueue requestQueue;
    QMutex mutex;
    QWaitCondition condition;
#endif
signals:
    void requestDequeue(const QRequestMsg& msg);
};

#endif //EOS_REQUESTQUEUETHREAD_H
