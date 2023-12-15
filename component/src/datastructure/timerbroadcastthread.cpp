/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-15 17:02:54
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 17:06:15
 * @FilePath: \EOS\component\src\datastructure\timerbroadcastthread.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "timerbroadcastthread.h"

void TimerBroadCastThread::run()
{
    while(true) {
        emit currentDateTime(QDateTime::currentDateTime());
        msleep(1000); // 1s一次即可
    }
}

TimerBroadCastThread::TimerBroadCastThread(QObject *parent) : QThread(parent)
{

}