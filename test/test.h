/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 09:08:50
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 09:14:07
 * @FilePath: \EOS\test\test.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_TEST_H
#define EOSI_TEST_H

#include "interface.h"
#include "socketpanel.h"

#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")

static void test_tcp()
{
    auto w = new SocketPanel;
    w->resize(800,600);
    w->show();

}

static void test_assemble_parse()
{ // 测试组装和解析和主线程是否相同,结果是不相同,且可以通过loop阻塞等待异步信号拿到结果
    LOG << "main thread: "<<QThread::currentThread();

    // (1) 组装
    QVariantMap m;
    m["0x0000"] = "0x0000";
    AssemblerPointer->assemble("0x0000",m);
    auto msg = AssemblerPointer->message();
    msg.chop(3);
    LOG<<"msg = "<<msg;

    // (2) 解析
    ParserControl p;
    p.parse("0x0000",msg);
    LOG<<"frame = "<<p.frame()<<" res = "<<p.result();
}

void test_pattern()
{
    auto w = new Pattern(16,24);
    w->toggleState(Pattern::TickState);
    w->selectPoints(QPointVector()<<QPoint(1,1)<<QPoint(2,2));
    w->resize(MainWindowWidth,MainWindowHeight);
    w->show();
}
#endif //EOSI_TEST_H
