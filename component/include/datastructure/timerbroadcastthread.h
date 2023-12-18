/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-15 16:57:06
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 17:06:01
 * @FilePath: \EOS\component\include\datastructure\timerbroadcastthread.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_TIMERBROADCASTTHREAD_H
#define EOS_TIMERBROADCASTTHREAD_H

#include "datastructure.h"
#include "constant.h"

class COMPONENT_IMEXPORT TimerBroadCastThread: public QThread
{
    Q_OBJECT
public:
    static TimerBroadCastThread& instance();
    void stopThread();
    void startThread();
private:
    explicit TimerBroadCastThread(QObject*parent= nullptr);
    void run() override;
    bool flag = false;
signals:
    void currentDateTime(const QDateTime& datetime);
};
#define TimerBroadCastThreadPointer (&TimerBroadCastThread::instance())
#endif //EOS_TIMERBROADCASTTHREAD_H
