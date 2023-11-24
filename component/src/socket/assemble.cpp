/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 17:27:25
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 17:27:54
 * @FilePath: \EOS\component\src\socket\assemble.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "assemble.h"

Assembler::Assembler(QObject *parent) : QObject(parent)
{

}

void Assembler::assemble(const QString&frame,const QVariantMap &m)
{ // 调用组装函数,把组装结果通过信号发出去
    QMutexLocker locker(&mutex);
    //LOG<<"current assemble thread is "<<CURRENT_THREAD;
    auto json = TcpAssembleFunctions[frame](m);
    emit assembleResult(json);
}

