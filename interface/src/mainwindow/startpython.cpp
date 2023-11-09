/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:42:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 10:42:59
 * @FilePath: \EOS\interface\include\startpython.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "startpython.h"

StartPython& StartPython::instance()
{
    static StartPython instance;
    return instance;
}

StartPython::StartPython(QObject *parent) : QObject(parent)
{
    thread = nullptr;
    SocketInit;
}

void StartPython::start(const QString &path, const QString &file, const QString &func)
{
    if (thread == nullptr)
    {
        thread = new PythonThread(path,file,func);
        //connect(thread,&PythonThread::finished,thread,&PythonThread::deleteLater);
    }

    if (!thread->isRunning())
        thread->start();
    SocketPointer->exec(TcpFramePool.frame0x0002,assemble0x0002(QVariantMap()));
    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
    else LOG<<"socket is connect failed!";
    SocketPointer->exec(TcpFramePool.frame0x0003,assemble0x0003(QVariantMap()));
    LOG<<"activate code is "<<ParserResult.toString();
}

void StartPython::quit()
{
    if (thread)
    {
        LOG<<"quit python thread";
        thread->setFlag();
//        thread->quit(); // 这样不能退出线程,只能通过setflag退出
//        thread->wait();
//        delete thread;
//        thread = nullptr;
    }
}

StartPython::~StartPython() noexcept
{
    if (thread)
    {
        LOG<<"delete python thread";
        thread->setFlag();
//        delete thread;
//        thread = nullptr;
    }
}
