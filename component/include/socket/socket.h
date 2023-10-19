/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 16:24:37
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 16:25:07
 * @FilePath: \EOS\component\include\socket\socket.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SOCKET_H
#define EOSI_SOCKET_H

// (1) 包含头文件
#include "qtcpsocket.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qdebug.h"
#include "qdatetime.h"
#include "qmutex.h"
#include "qthread.h"
#include "alias.h"

// (2) 定义常量
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")
#define SqlExecFailedLOG (qDebug()<<"[" \
    <<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] sql exec failed! error is ")
#define SocketPort 3000
#define LocalHost "localhost"
static const char* FrameField = "frame";
static const char* SeparateField = "@@@";
#define AppendSeparateField(s) (s+SeparateField)

class Socket;
#define SocketInit (TcpSocket::instance())
#define SocketPointer (&TcpSocket::instance())

class AssemblerControl;
#define AssemblerPointer (&AssemblerControl::instance())
#define AssemblerMessage (AssemblerPointer->message())

// (3) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif

typedef struct { // 注册过的帧头命令
    const QString frame0x0000 = "0x0000";
    const QString frame0x0001 = "0x0001";
    const QString frame0x0002 = "0x0002";
    const QString frame0x0003 = "0x0003";
    const QString frame0x0004 = "0x0004";
    const QString frame0x1000 = "0x1000";
} TcpFrameList;

static QJsonDocument TcpAssemblerDoc;
static const TcpFrameList TcpFramePool;

static QVariant parse0x0000(QCVariantMap m)
{
    Q_UNUSED(m);
    QVariant d;
    return d;
}

static QByteArray assemble0x0000(QCVariantMap m)
{
    Q_UNUSED(m);
    QJsonObject object;
    object[FrameField] = TcpFramePool.frame0x0000;

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

/*---------以下都是临时测试函数,以后可以注释掉-----------------*/
static QByteArray assemble_test0x0(QCVariantMap m)
{ // test0x0会传来x,y,frame字段
    QJsonObject object;
    object[FrameField] = "test0x0";
    object["x"] = m["x"].toString();
    object["y"] = m["y"].toString();

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}
/*---------以上都是临时测试函数,以后可以注释掉-----------------*/

// 根据帧头选择对应的解析函数
static QMap<QString,TcpParseFuncPointer>  TcpParseFunctions = {
        {TcpFramePool.frame0x0000,parse0x0000},
//        {TcpFramePool.frame0x0001,parse0x0001},
//        {TcpFramePool.frame0x0002,parse0x0002},
//        {TcpFramePool.frame0x0003,parse0x0003},
//        {TcpFramePool.frame0x0004,parse0x0004},
//        {TcpFramePool.frame0x1000,parse0x1000},
};

static QMap<QString,TcpAssembleFuncPointer>  TcpAssembleFunctions = {
        {TcpFramePool.frame0x0000,assemble0x0000},
//        {TcpFramePool.frame0x0001,assemble0x0001},
//        {TcpFramePool.frame0x0002,assemble0x0002},
//        {TcpFramePool.frame0x0003,assemble0x0003},
//        {TcpFramePool.frame0x0004,assemble0x0004},
//        {TcpFramePool.frame0x1000,assemble0x1000},

        {"test0x0",assemble_test0x0},
};


#endif //EOSI_SOCKET_H
