/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:52:58
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:35
 * @FilePath: \EOS\component\src\datastructure\eventloop.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "eventloop.h"

EventLoop::EventLoop(QObject *parent) : QObject(parent)
{

}

void EventLoop::quit()
{
    loop.quit();
}

void EventLoop::exec()
{
    emit started();
    loop.exec();
}
