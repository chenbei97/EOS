/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 08:34:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 08:43:19
 * @FilePath: \EOS\component\src\socket\parse.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "parse.h"

void Parse::parse(const QString&f,const QByteArray &msg)
{
    QMutexLocker locker(&mutex);
    //LOG<<"current parse thread is "<<CURRENT_THREAD;
    //LOG<<"parse msg = "<<msg;
    JsonReadWrite m;
    m.parseJson(msg);
    auto map = m.map();

    if (map.isEmpty()) { // 不排除解析的json格式有问题无法解析
        emit parseResult(f,QVariant()); // 有错误的话,解析结果要传递
        return;
    }

    auto keys = map.keys();

    if (!keys.contains(FrameField)){
        emit parseResult(f,QVariant());
        return;
    }

    auto frame = map[FrameField].toString(); // 返回消息的帧

    if (f != frame ) {// 返回和发送帧不等(有可能是多条命令的混合返回另一条命令的结果)
        auto result = TcpParseFunctions[frame](map); //这个结果也要反馈出去
        if (result.isNull()) { // 以实际帧frame为准
            emit parseResult(frame,QVariant());
            return;// 如果这个结果还不能正确解析就是帧错误
        }
        emit parseResult(frame,result);
        return;
    }

    auto d = TcpParseFunctions[frame](map); // 调用函数指针解析
    emit parseResult(frame,d);
}

Parse::Parse(QObject *parent) : QObject(parent)
{
}
