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
#include <thread>
using std::async;
using std::promise;
using std::future;

#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")

static void test_async()
{
// promise<int> pval;
// pval.set_value(-1);
// auto func1 = [](promise<int> & p){
//     //p.set_value_at_thread_exit(9);
//     p.set_value(9);
//
// };
// std::thread thread1(func1,std::ref(pval));
// LOG<<"is joinable? "<<thread1.joinable();
// //thread1.join();
// thread1.detach();
// auto future1 = pval.get_future();
// LOG<< "promise val = "<<future1.get();


 auto func2 = []()->int{
     int val = -1;
     LOG<<"before sleep_for val = "<<val;
     std::this_thread::sleep_for(std::chrono::seconds(3));
     val = 9;
     LOG<<"after sleep_for val = "<<val;
     return val;
 };
    std::future<int> future2 = std::async(std::launch::async, func2);
    std::future_status status;
    do {
        status = future2.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred) {
            LOG << "deferred\n";
        } else if (status == std::future_status::timeout) {
            LOG << "timeout\n";
        } else if (status == std::future_status::ready) {
            LOG << "ready!\n";
        }
    } while (status != std::future_status::ready);
    LOG << "result is " << future2.get() << '\n';
}

static void test_tcp()
{
    auto w = new SocketPanel;
    w->resize(1000,600);
    setWindowAlignCenter(w);
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
    ParserPointer->parse("0x0000",msg);
    LOG<<"frame = "<<ParserFrame<<" res = "<<ParserResult;
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
