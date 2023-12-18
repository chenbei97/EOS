/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-15 17:02:54
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 17:06:15
 * @FilePath: \EOS\component\src\datastructure\timerbroadcastthread.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "timerbroadcastthread.h"

TimerBroadCastThread &TimerBroadCastThread::instance()
{
    static TimerBroadCastThread instance;
    return instance;
}

void TimerBroadCastThread::run()
{
    LOG<<"timer broadcast's thread: "<<CURRENT_THREAD;
    while(flag) {
        emit currentDateTime(QDateTime::currentDateTime());
        //LOG<<"here";
        msleep(1000); // 1s一次即可
    }
}

TimerBroadCastThread::TimerBroadCastThread(QObject *parent) : QThread(parent)
{

}

void TimerBroadCastThread::startThread()
{
    flag = true;
    start();
}

void TimerBroadCastThread::stopThread()
{
    flag = false;
    quit();
    wait();
}