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
#ifdef use_msgqueue_v1
        message = socket->readAll();
        // 这种写法一般来说不会有问题,只要服务端的代码不会让socket同时出现多条命令或者不完整命令就不会出错,一收一发式的理想情况
        auto msgs = QString::fromUtf8(message).split(SeparateField,QString::SkipEmptyParts);
        //LOG<<"response msg  = "<<msgs;
        foreach(auto m ,msgs)
            msgQueue.enqueue(m.toUtf8()); // 将消息添加到队列中
#else
        message += socket->readAll();
        int count = message.count(SeparateField);//出现分隔符就知道有count条完整的消息出现
        //LOG<<"before: "<<message<<" count = "<<count;
        while (count--) {
            auto len = message.indexOf(SeparateField,0); //从最左边的完整消息结束符开始
            auto response = message.left(len); // 最左边的完整消息
            msgQueue.enqueue(response); // 去处理
            message.remove(0,len); // 把已经处理的消息和分隔符去掉,处理下一轮
            message.remove(0,QString(SeparateField).count());
        }
        //LOG<<"after: "<<message;
#endif
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

         readAll可能的情况如下(对EOS来说没有复杂的高并发,socket都是一收一发式的不会出现粘包,但应该有安全保障机制):
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

            一般来说对于EOS都是[F1][F2][se][p]这样理想的情况,不会出现问题;
            客户端传给服务端已经引入队列机制,不会出现滑动条移动让socket瞬时有n条消息,否则服务端必须考虑粘包的情况(而且服务端也本应该建立这种安全机制,即使不出现)
            服务端给客户端,目前data界面还没设计,尚未知道服务端给客户端是如何发送消息的,所以要考虑服务端没有使用消息队列给客户端发消息时粘包的处理
            除了上述16种情况,还有12种情况,也就是还要考虑3,2,1份的组合,上边是4个进行组合
            (5) [F1][F2][se],[F2][se][p],[se][p][F1],[p][F1][F2] 其余的12种情况
            (6) [F1][F2] [F2][se] [se][p] [p][F1]
            (7) [F1] [F2] [se] [p]

            不论是以上哪种情况,其实就是考虑是否出现一个完整命令[F1][F2][se][p],从首次readAll开始一定是[F1]开始
            如果没有遇到完整的sep,就让消息先进入缓存,直到出现sep取出左边的完整命令,清理缓存,也就是可以先计算sep出现的次数
            以下的伪代码就是这样的一种保障机制,实际上这部分工作服务端就应该做好不允许多条命令或者不完整的命令写入套接字,
            或者说是服务端有责任和义务避免多条命令或者不完整的命令同时写入套接字造成客户端粘包,但是客户端也可以建立安全机制以防服务端没有做这种工作
            伪代码:
            buffer += socket->readAll();
            int count = buffer.count(sep);
            while (count--) { // 可能出现2个以上
                auto idx = buffer.indexOf(sep,0); // idx是sep中s的位置,from=0,先处理最左边的完整命令
                auto response = buffer.left(idx); // idx也恰好是sep前边(不包含s)的字符数量,得到完整的请求命令
                msgQueue.enqueue(response); // 放入回复消息队列处理
                buffer.remove(0,idx); // 移除[F1][F2]
                buffer.remove(0,sep.count());// sep也要移除
            }
            测试情况:
            将sscom调为TcpServer格式,启动EOS,sscom循环给客户端发送5条消息
            {"frame":"2","state":3}@@@{"frame":"2","state":3}@@@
            {"frame":"2",
            "state":3}@@@
            {"frame":"2","state":3}@
            @@
            会有以下打印结果,符合预期:
            before:  "{\"frame\":\"2\",\"state\":3}@@@{\"frame\":\"2\",\"state\":3}@@@"  count =  2
            after:  "" 第1条发送了完整的2条消息,所以都解析了
            before:  "{\"frame\":\"2\","  count =  0
            after:  "{\"frame\":\"2\"," 第2条
            before:  "{\"frame\":\"2\",\"state\":3}@@@"  count =  1
            after:  "" 和第3条合并才是完整的
            before:  "{\"frame\":\"2\",\"state\":3}@"  count =  0
            after:  "{\"frame\":\"2\",\"state\":3}@" 第4条
            before:  "{\"frame\":\"2\",\"state\":3}@@@"  count =  1
            after:  "" 和第5条合并才是完整的
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
{ // 使用定时器方式 处理请求消息队列(占据cpu高)
    while (!requestQueue.empty()) {
        //LOG<<"requestQueue's count = "<<requestQueue.count();
        frame = requestQueue.head().first;
        auto request = requestQueue.head().second;
        requestQueue.dequeue();
        LOG<<"process request "<<request;
        socket->write(request);
        //LOG<<"before loop is running? "<<loop.isRunning();//可以看出每次调用前exec已经退出
        loop.exec();
        //LOG<<"after loop is running? "<<loop.isRunning();
    }
}

void TcpSocket::processRequestMsg(const QRequestMsg& msg)
{ // 使用子线程方式处理请求消息队列 (代替定时器方式)
    exec(msg.first,msg.second,false); // 子线程固定100ms处理1次,其实就不能使用同步逻辑了
}

void TcpSocket::exec(const QString& f,const QByteArray& c,bool use_sync)
{  // 本函数适用于：1.服务回复消息无明显延迟的同步;2.非短时调用;3.异步;
    // 1.询问激活码; 2.询问连接状态; 3.切换通道开关灯
    //LOG<<"request msg = "<<c;
    /* v1写法: 对于使用同步,如果exec在慢速被调用时没有任何问题,如果被快速的高频调用(目前有滑动条的情况,4个三角快速微调镜头,预览事件等,后2者经实际测试也是反应过来的不需要请求队列)
     * 以下代码写法会出现 0x7ffeab390740 has already called exec()这样的警告,也就是提升loop已经被调用,尚未退出又被调用,因为loop作用域是类内部只有1个实例
     * 其实这个警告不会影响实际功能,只是系统压入栈需要时间处理频繁的loop被调用然后释放本地循环
     * exec_queue的写法是针对请求消息可以使用队列机制就可以避免loop被频繁调用,1条条处理队列的请求消息(一般情况用exec就可以)
     * 或者说是客户端有责任和义务避免多条命令或者不完整的命令同时写入套接字造成服务端粘包
     * ※※※※ 如果服务端回复消息有明显延迟,例如预览事件点击孔电机尚未移动到位又点击不适用本函数 ※※※※
     * 除了造成has already called exec(),同时PareseResult.toBool()消息会错乱
        [ "14:11:27:307" Preview::previewHoleEvent ]  exec  QPoint(1,0) 这里点了1下
        [ "14:11:29:171" Preview::previewHoleEvent ]  exec  QPoint(1,1) 电机不到位还没回复又点了一下
        QEventLoop::exec: instance 0x7ffd75d63c90 has already called exec() 弹出警告
        [ "14:11:29:172" Preview::previewHoleEvent ]  move to hole point  QPoint(1,1)  failed! 其实这里应该是(1,0)失败
        [ "14:11:32:332" Preview::previewHoleEvent ]  move to hole point  QPoint(1,1)  successful! 然后(1,1)成功是对的
     * loop是个比较"软"的阻塞而不是"强"阻塞,因为可以二次被调用,所以这不适用有明显延迟的情况,另一个版本的exec使用对话框的exec来强阻塞代替loop
     * */

    frame = f;
    socket->write(c);
    //LOG<<"loop is running? "<<loop.isRunning(); // 使用本函数快速滑slider会出现 "loop is running?  true"
    if (use_sync) // 使用同步
        loop.exec(); // 阻塞直到解析完,此时TcpSocket或者ParsePointer拿到结果
}

void TcpSocket::exec(const QString& f,const QByteArray& c)
{ // 本函数只适用于: 有弹窗需求的硬同步,服务侧有明显延迟;
    // 适用于:1.预览孔/视野/载玻片事件;2.微调镜头事件; 3切换物镜同时动电机;
    // 4.setting单纯动电机到某个位置; 5.启动实验 6.手动调焦 7.载玻片的拼图
    // 8.切换通道也能用
    frame = f;
    socket->write(c);
    looptimer.start(SocketWaitTime);
    if (f == TcpFramePool.previewEvent || f == TcpFramePool.adjustLensEvent || f == TcpFramePool.toggleObjectiveEvent
     || f == TcpFramePool.moveMachineEvent || f == TcpFramePool.manualFocusEvent) {
        waitdlg->setWaitText(tr(WaitMessageBoxMoveMachineMsg));
    } else if (f == TcpFramePool.loadExperEvent) {
        waitdlg->setWaitText(tr(WaitMessageBoxStartExperimentMsg));
    } else if ( f == TcpFramePool.stopExperEvent) {
        waitdlg->setWaitText(tr(WaitMessageBoxStopExperimentMsg));
    } else if (f == TcpFramePool.toggleChannelEvent) {
        waitdlg->setWaitText(tr(WaitMessageBoxToggleChannelMsg));
    } else if (f == "test0x3") {
        waitdlg->setWaitText(tr(WaitMessageBoxWaitStitchMsg));
    }
    waitdlg->wait(); // 弹窗形式的硬同步
}

void TcpSocket::exec(const QString& f,const QByteArray& c,unsigned count)
{ // 本函数只适用于: 有弹窗需求的硬同步,服务侧有明显延迟,且是需要多轮发送与回复的流程;
    // 适用于:1.自动聚焦 2.载玻片的拼图
    frame = f;
    socket->write(c);
    looptimer.start(SocketWaitTime);
    if (f == "test0x2" || f == TcpFramePool.autoFocusEvent)
        waitdlg->setWaitText(tr(WaitMessageBoxAutoFocusMsg).arg(count+1));
    if (f == "test0x3") {
        waitdlg->setWaitText(tr(WaitMessageBoxSlideStitchMsg).arg(count+1));
    }
    waitdlg->wait(count);
}

void TcpSocket::exec_queue(const QString &f, const QByteArray &c)
{//使用请求消息队列的方式: 本函数用于滑动条或者短时发送大量请求消息的同步操作,而且不需要弹窗硬同步
    // 1.调节br
    //LOG<<"request msg = "<<c;

#ifdef use_queuethread
    requestThread->enqueue(qMakePair(f,c));
#else
    //LOG<<"before = "<<requestQueue.count();
    requestQueue.enqueue(qMakePair(f,c));
    //LOG<<"after = "<<requestQueue.count();
    /*
     *  使用本函数放入队列,快速滑slider时队列会逐个去请求,不会重复调用loop.exec
     *  然后定时器触发processRequestQueue逐个的去处理,并且从loop的running打印结果可以看出服务端收到解析信号就及时退出了loop
     *  下次调用不会出现loop已经处于running的状态,也就不会出现V1写法的警告 has already called exec()
     *  不过要注意: loop.exec是在定时器的异步操作内执行的,而不是在exec_queue上下问函数立即执行的
     *  所以会出现:如果只调用1次exec_queue时ParseResult没有立即同步的现象,例如main.cpp询问是否连接和激活码
     *  所以如果只需要执行1次的操作请使用exec而不是exec_queue,或者使用exec_queue后立即调用下方语句才可
     *  QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
     *  main.cpp询问是否连接和激活码的代码,使用2种方式均可
     *  SocketPointer->exec(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()),true);
        SocketPointer->exec_queue(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()));
        QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
        if (ParserResult.toBool()) LOG<<"socket is connect successful!";
        else LOG<<"socket is connect failed!";
     *  SocketPointer->exec(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()),true);
        SocketPointer->exec_queue(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()));
        QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
        LOG<<"activate code is "<<ParserResult.toString();
        [ "11:34:38:348" TcpSocket::exec ]  before =  3 // 快速滑动时processRequestQueue的执行晚于exec,故请求队列开始增加,积累请求
        [ "11:34:38:348" TcpSocket::exec ]  after =  4
        [ "11:34:38:349" TcpSocket::exec ]  before =  4
        [ "11:34:38:350" TcpSocket::exec ]  after =  5
        [ "11:34:38:352" TcpSocket::exec ]  before =  5
        [ "11:34:38:352" TcpSocket::exec ]  after =  6
        [ "11:34:38:354" TcpSocket::exec ]  before =  6
        [ "11:34:38:355" TcpSocket::exec ]  after =  7
        .... // 之后定时器异步的起作用逐个处理,请求队列减少
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
#ifdef use_testport
    port = SocketTestPort;
#endif
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

void TcpSocket::resetWaitText()
{
    waitdlg->setWaitText(tr(WaitMessageBoxMoveMachineMsg));
}

void TcpSocket::setWaitText(const QString &text)
{
    waitdlg->setWaitText(text);
}

TcpSocket::TcpSocket(QObject *parent):QObject(parent)
{
    socket = new QTcpSocket;
    //waitdlg = getWaitMessageBox(tr(WaitMessageBoxMoveMachineMsg),false);
    waitdlg = new WaitMessageBox(tr(WaitMessageBoxMoveMachineMsg));
    socket->setReadBufferSize(0);// 默认就是0,也就是缓冲不受限制,有多少就接受多少
    socket->setSocketOption(QAbstractSocket::LowDelayOption, true); // 尽可能低延迟,但是不能避免

    looptimer.setSingleShot(true);
    looptimer.setInterval(SocketWaitTime);

    // 软同步: loop只要执行就启动定时器,如果一直没有回复也不能卡着,定时器超时就退出loop
    connect(&looptimer,&QTimer::timeout,&loop,&EventLoop::quit);
    connect(&loop,&EventLoop::started,this,[=]{looptimer.start(SocketWaitTime);});
    //拿到同步后的结果,外接可以从TcpSocket.result也可以直接从ParserResult拿结果,这2个都已经是同步的
    connect(ParserPointer,&ParserControl::parseResult,&loop,&EventLoop::quit);

    // 硬同步: 超时就弹窗阻塞
    connect(ParserPointer,&ParserControl::parseResult,waitdlg,&QMessageBox::accept);
    connect(&looptimer,&QTimer::timeout,waitdlg,&QMessageBox::accept);

    connect(socket,&QTcpSocket::readyRead,this,&TcpSocket::onReadyReadSlot);
    connect(socket,&QTcpSocket::readyRead,this,&TcpSocket::processMsgQueue);

    // 队列同步: 定时器和子线程方式,但是定时器占据cpu高不采用
#ifdef use_queuethread
    requestThread = new RequestQueueThread(this);
    connect(requestThread,&RequestQueueThread::requestDequeue,this,&TcpSocket::processRequestMsg);
    requestThread->start();
#else
    connect(&requesttimer,&QTimer::timeout,this,&TcpSocket::processRequestQueue);
    requesttimer.start(0);
#endif
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
#ifdef use_queuethread
    if (requestThread->isRunning()) {
        requestThread->quit();
        requestThread->wait();
    }
#endif
}