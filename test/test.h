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

#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")


static void test_assemble_parse()
{
    LOG << "main thread: "<<QThread::currentThread();
    AssemblerController c;
    QVariantMap m;
    m["0x0000"] = "0x0000";
    c.assemble("0x0000",m);
    auto msg = c.message();
    msg.chop(3);
    LOG<<"msg = "<<msg;

    ParseControl p;
    p.parse("0x0000",msg);
    LOG<<"frame = "<<p.frame()<<" res = "<<p.result();
}
#endif //EOSI_TEST_H
