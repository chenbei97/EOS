/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 17:26:38
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 17:27:58
 * @FilePath: \EOS\component\src\socket\assemblecontrol.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "assemblecontrol.h"

AssemblerController::AssemblerController(QObject *parent) : QObject(parent)
{
    assembler = new Assembler;
    assembler->moveToThread(&assemblethread);
    // 组装信号传递给组装类,触发信号和同步信号,这样信号回来后这里message()就可以拿到同步后的结果
    connect(this,&AssemblerController::assemble,assembler,&Assembler::assemble);
    connect(this,&AssemblerController::assemble,this,[&]{loop.exec();});//组装时同步等待异步的assembleFinished
    connect(assembler,&Assembler::assembleResult,this,[&](const QByteArray& m){msg=m;emit assembleFinished();});

    connect(this,&AssemblerController::assembleFinished,&loop,&EventLoop::quit);
    assemblethread.start();
}

QByteArray AssemblerController::message() const
{
    return msg;
}

AssemblerController::~AssemblerController()
{
    assemblethread.quit();
    assemblethread.wait();
}
