/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 16:21:42
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 11:54:11
 * @FilePath: \EOS\component\src\socket\tcpsocket.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "tcpsocket.h"

void TcpSocket::onReadyReadSlot()
{
    while (socket->bytesAvailable()) { // 有效就都读出来了
        QByteArray msg = socket->readAll();
        auto msgs = QString::fromUtf8(msg).split(SeparateField,QString::SkipEmptyParts);
        //LOG<<"response msg  = "<<msgs;
        foreach(auto m ,msgs)
            msgQueue.enqueue(m.toUtf8()); // 将消息添加到队列中
        /*
        Tcp粘包问题，粘包问题参考网站解释,比较清楚明白，并不是发送方发5次就会触发readyRead5次
            1). https://blog.csdn.net/dengdew/article/details/79065608
            2). https://blog.csdn.net/yangzijiangtou/article/details/99676692
        粘包原因: 1. 发送端需要等缓冲区满了才发送出去或者发送太快，造成粘包
                 2. 接收方不及时接收缓冲区的包，造成多个包一起接收。
                 3. 不是接收方问题，系统TCP/IP发现数据过快变为超时或者缓冲区满才发给接收方
        解决方法: 1. 发送方不要等缓冲区满了才发送，通过while实现写入套接字消息就发出；如果发送太快也不行，使用sleep延迟发送
                 2. 接收方通过while实现有套接字消息就读出，然后解析
                 3. 为了避免数据混乱，在消息结尾或者开头加入特定字符串表示一个包的开始和结束，来划分不同的包
                 4. 操作中继者,系统TCP/IP，让它不必超时或者缓存满了才发出，有消息就发出（暂时没找到方法）
                 5. socket->setSocketOption(QAbstractSocket::LowDelayOption, true); 一种手段但是不能避免
         */
    }
}

void TcpSocket::processMsgQueue()
{
    while (!msgQueue.isEmpty()) {
        auto message = msgQueue.head();
        ParserPointer->parse(frame,message);
        msgQueue.dequeue(); // 从队列中取出消息
    }
}

void TcpSocket::exec(const QString& f,const QByteArray& c,bool user_sync)
{
    LOG<<"request msg = "<<c;
    frame = f;
    socket->write(c);
    if (user_sync) // 使用同步
        loop.exec(); // 阻塞直到解析完,此时TcpSocket或者ParsePointer拿到结果
}

QVariantMap TcpSocket::result() const
{
    QVariantMap m;
    m[ParserFrame] = ParserResult;
    return m;
}

TcpSocket& TcpSocket::instance()
{
    static TcpSocket s;
    return s;
}

void TcpSocket::connectToHost(const QString &hostName, quint16 port)
{
    socket->connectToHost(hostName,port,QIODevice::ReadWrite);
}

TcpSocket::TcpSocket(QObject *parent):QObject(parent)
{
    socket = new QTcpSocket;
    socket->setReadBufferSize(0);// 默认就是0,也就是缓冲不受限制,有多少就接受多少
    socket->setSocketOption(QAbstractSocket::LowDelayOption, true); // 尽可能低延迟,但是不能避免,还是会粘包

    timer.setSingleShot(true);
    timer.setInterval(SocketWaitTime);
    // loop只要执行就启动定时器,如果一直没有回复也不能卡着,定时器超时就退出loop
    connect(&timer,&QTimer::timeout,&loop,&EventLoop::quit);
    connect(&loop,&EventLoop::started,this,[=]{timer.start(SocketWaitTime);});

    connect(socket,&QTcpSocket::readyRead,this,&TcpSocket::onReadyReadSlot);
    connect(socket,&QTcpSocket::readyRead,this,&TcpSocket::processMsgQueue);
    //connect(socket,&QTcpSocket::readyRead,&loop,&EventLoop::exec);
    connect(ParserPointer,&ParserControl::parseResult,&loop,&EventLoop::quit);
    connectToHost();

}

TcpSocket::~TcpSocket()
{
    if(socket) {
        socket->disconnectFromHost();
        socket->close();
        delete socket;
        socket = nullptr;
    }
}