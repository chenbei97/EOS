/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-09 15:04:36
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-09 16:36:25
 * @FilePath: \EOS\interface\src\mainwindow\pythonprocess.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "pythonprocess.h"

PythonProcess::PythonProcess(QObject*parent): QObject(parent)
{
    SocketInit;
    process = new QProcess;
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [this]() {
        process->close();
        process->waitForFinished();
        LOG<<"python process is kill? "<<!process->isOpen();
    });
}

void PythonProcess::start(const QString& path)
{
    process->start("python",QStringList()<<path);
    process->waitForStarted();
    SocketPointer->exec(TcpFramePool.frame0x0002,assemble0x0002(QVariantMap()));
    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
    else LOG<<"socket is connect failed!";
    SocketPointer->exec(TcpFramePool.frame0x0003,assemble0x0003(QVariantMap()));
    LOG<<"activate code is "<<ParserResult.toString();
}
