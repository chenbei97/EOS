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
        readAll可能的情况如下:
            其中F1F2表示完整的文本内容,sep表示分隔符,分隔符可能不完整
            所以数学上可以看成是[F1][F2][se][p]的组合 但是有些限制,[p]只能在[se]后面,[F2]后边只能是[se]等这种顺序限制
            如果没有限制,数学上4个位置的全排列是4!=24种,现在其实是16种
            (1) 1.[F1] 2.[F1][F2] 3.[F1][F2][se] 4.[F1][F2][se][p] 内容的前一部分开头
                [F1][F2][se][p][F1] 这里从[F1]进行重复,实际4种情形
            (2) 1.[F2] 2.[F2][se] 3.[F2][se][p] 4.[F2][se][p][F1] 内容的后一部分开头
                [F2][se][p][F1][F2] 这里从[F2]进行重复,实际4种情形
            (3) 1.[se] 2.[se][p] 3.[se][p][F1] 4.[se][p][F1][F2] 不完整分隔符的前一部分,当然可能是s,也可能se
                [se][p][F1][F2][se] 这里从[se]进行重复,实际4种情形
            (4) 1.[p] 2.[p][F1] 3.[p][F1][F2] 4.[p][F1][F2][se] 不完整分隔符的后一部分,当然可能是p,也可能ep
                [p][F1][F2][se][p] 这里从[p]进行重复,实际4种情形
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

void TcpSocket::processRequestQueue()
{
    while (!requestQueue.empty()) {
        //LOG<<"requestQueue's count = "<<requestQueue.count();
        frame = requestQueue.head().first;
        auto request = requestQueue.head().second;
        requestQueue.dequeue();
        //LOG<<"process request "<<request;
        socket->write(request);
        //LOG<<"before loop is running? "<<loop.isRunning();
        loop.exec();
        //LOG<<"after loop is running? "<<loop.isRunning();
    }
}

#define user_v2_exec 0
void TcpSocket::exec(const QString& f,const QByteArray& c,bool use_sync)
{
    //LOG<<"request msg = "<<c;
    /* v1写法: 对于使用同步,如果exec在慢速被调用时没有任何问题,如果被快速的高频调用(目前只有滑动条的情况)
     * 以下代码写法会出现 0x7ffeab390740 has already called exec()这样的警告,也就是提升loop已经被调用,尚未退出又被调用,因为loop作用域是类内部只有1个实例
     * 其实这个警告不会影响实际功能,只是系统压入栈需要时间处理频繁的loop被调用然后释放本地循环
     * 下方新代码的写法是针对请求消息可以使用队列机制就可以避免loop被频繁调用,1条条处理队列的请求消息
     frame = f;
     socket->write(c);
    LOG<<"loop is running? "<<loop.isRunning(); // 快速滑slider会出现 "loop is running?  true"
     if (use_sync) // 使用同步
        loop.exec(); // 阻塞直到解析完,此时TcpSocket或者ParsePointer拿到结果
     * */

#ifdef user_v1_exec
    frame = f;
    socket->write(c);
    LOG<<"loop is running? "<<loop.isRunning(); // 快速滑slider会出现 "loop is running?  true"
    if (use_sync) // 使用同步
        loop.exec(); // 阻塞直到解析完,此时TcpSocket或者ParsePointer拿到结果
#else
    /*v2写法: 如果是同步则引入请求消息队列*/
    if (!use_sync) {
        frame = f;
        socket->write(c); // 如果使用异步不需要队列,结果需要根据Parse类的异步信号去拿结果
    } else {
        //LOG<<"request msg = "<<c;
        //LOG<<"before = "<<requestQueue.count();
        requestQueue.enqueue(qMakePair(f,c));
        //LOG<<"after = "<<requestQueue.count();
        /*
         *  放入队列,快速滑slider时队列会逐个去请求,不会重复调用loop.exec
         *  然后定时器触发processRequestQueue逐个的去处理,并且从loop的running打印结果可以看出服务端收到解析信号就及时退出了loop
         *  下次调用不会出现loop已经处于running的状态,也就不会出现V1写法的警告 has already called exec()
            [ "11:34:38:348" TcpSocket::exec ]  before =  3
            [ "11:34:38:348" TcpSocket::exec ]  after =  4
            [ "11:34:38:349" TcpSocket::exec ]  before =  4
            [ "11:34:38:350" TcpSocket::exec ]  after =  5
            [ "11:34:38:352" TcpSocket::exec ]  before =  5
            [ "11:34:38:352" TcpSocket::exec ]  after =  6
            [ "11:34:38:354" TcpSocket::exec ]  before =  6
            [ "11:34:38:355" TcpSocket::exec ]  after =  7
            ....
            [ "11:37:47:596" TcpSocket::processRequestQueue ]  requestQueue's count =  12 //这里看出loop下次exec前已经quit
            [ "11:39:25:431" TcpSocket::processRequestQueue ]  before loop is running?  false
            [ "11:39:25:439" TcpSocket::processRequestQueue ]  after loop is running?  false
            [ "11:37:47:598" TcpSocket::processRequestQueue ]  requestQueue's count =  11
            [ "11:39:25:431" TcpSocket::processRequestQueue ]  before loop is running?  false
            [ "11:39:25:439" TcpSocket::processRequestQueue ]  after loop is running?  false
            [ "11:37:47:599" TcpSocket::processRequestQueue ]  requestQueue's count =  10
            [ "11:39:25:431" TcpSocket::processRequestQueue ]  before loop is running?  false
            [ "11:39:25:439" TcpSocket::processRequestQueue ]  after loop is running?  false
            [ "11:37:47:600" TcpSocket::processRequestQueue ]  requestQueue's count =  9
            [ "11:39:25:431" TcpSocket::processRequestQueue ]  before loop is running?  false
            [ "11:39:25:439" TcpSocket::processRequestQueue ]  after loop is running?  false
         * */
    }
#endif
}

QVariantMap TcpSocket::result() const
{
    QVariantMap m;
    m[ParserFrame] = ParserResult;
    //LOG<<"result = "<<ParserResult;
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
    socket->waitForConnected(SocketWaitTime);//等待连接上
}

bool TcpSocket::isConnected() const
{
    return socket->state() == QTcpSocket::ConnectedState;
}

QTcpSocket::SocketState TcpSocket::socketState() const
{
    return socket->state();
}

TcpSocket::TcpSocket(QObject *parent):QObject(parent)
{
    socket = new QTcpSocket;
    socket->setReadBufferSize(0);// 默认就是0,也就是缓冲不受限制,有多少就接受多少
    socket->setSocketOption(QAbstractSocket::LowDelayOption, true); // 尽可能低延迟,但是不能避免,还是会粘包

    looptimer.setSingleShot(true);
    looptimer.setInterval(SocketWaitTime);
    // loop只要执行就启动定时器,如果一直没有回复也不能卡着,定时器超时就退出loop
    connect(&looptimer,&QTimer::timeout,&loop,&EventLoop::quit);
    connect(&loop,&EventLoop::started,this,[=]{looptimer.start(SocketWaitTime);});

    connect(socket,&QTcpSocket::readyRead,this,&TcpSocket::onReadyReadSlot);
    connect(socket,&QTcpSocket::readyRead,this,&TcpSocket::processMsgQueue);
    //拿到同步后的结果,外接可以从TcpSocket.result也可以直接从ParserResult拿结果,这2个都已经是同步的
    connect(ParserPointer,&ParserControl::parseResult,&loop,&EventLoop::quit);

    connect(&requesttimer,&QTimer::timeout,this,&TcpSocket::processRequestQueue);
    requesttimer.start(0);
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