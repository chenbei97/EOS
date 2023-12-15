/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 13:09:20
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 14:06:33
 * @FilePath: \EOS\main.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "test/test.h"

void initApp(QApplication& a);
#define usetesttcp 0

int main(int argc, char *argv[]) {
    LogInit;
    QApplication a(argc, argv);
    initApp(a);
#ifdef usetesttcp
    test_tcp();
#else
    MainWindow w;
    setWindowAlignCenter(&w);
    w.show();
#endif
    return QApplication::exec();
}

void initApp(QApplication& a)
{
    LOG << "UserScreen Width:" << UserScreenWidth << "UserScreen Height:" << UserScreenHeight;
    LOG<<"main thread is "<<CURRENT_THREAD;
    LOG<<"current path = "<<CURRENT_PATH;
//    foreach(auto p, systemDrivers()) {
//        LOG<<p<<" total = "<<getDiskSpace(p,false)<<"MB free = "<<getDiskSpace(p)<<"MB "
//        << getDiskSpace(p) / getDiskSpace(p,false) * 100.0 <<"%";
//    }
    //QTextCodec *codec = QTextCodec::codecForName("utf-8");
    //QTextCodec::setCodecForLocale(codec);
    a.setFont(QFont(DefaultFontFamily,DefaultFontSize));
    a.setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DesktopIcon));
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    //QTimer::singleShot(0,qApp,SLOT(quit()));

    // 这段Python进程的代码移动到initApp去写而不是在mainwindow.cpp的构造函数去写
    // 因为mainwindow.cpp构造完成之前可能已经触发了某些信号给服务端发送命令,例如独立于mainwindow构造的setting
    // 这样setting有关的信号发送的时候python进程尚未启动(mainwindow并未构造完)导致服务端没收到消息
    // 所以提前启动Python进程,这样mainwindow或者settng初始化发送某些信号时都能保证服务端已经正常
    // 现在方便测试自己去启动Python,以后变成自启动时这些代码也就不需要了,目前需要保留

    // 1.进程方式
    //auto process = new PythonProcess;
    //process->start("../test/test_socket.py");
    // 2.线程方式
    //StartPythonPointer->start("../test","test_socket","test_server");

    // 3.非线程也非进程方式(会阻塞)
    //PythonCallPointer->start("../test","test_socket","test_server");

    // 属于进程方式 测试方便,暂时需要保留,以后可以从这开始到代码结束都注释掉
    auto * process = new QProcess;
    // 22987 Lenovo
    process->start("C:\\Users\\22987\\AppData\\Local\\Programs\\Python\\Python310\\python.exe",
                   QStringList()<<"../test/test_socket.py");
    process->waitForStarted();
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [process]() {
        process->close();
        process->waitForFinished();
        LOG<<"python process is kill? "<<!process->isOpen();
    });
    SocketInit; //注意: 先启动进程再启动连接
    //LOG<<"is connect? "<<SocketPointer->isConnected()<<SocketPointer->socketState();

    SocketPointer->exec(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()),true);
    //SocketPointer->exec_queue(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()));
    //QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
    else LOG<<"socket is connect failed!";

    SocketPointer->exec(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()),true);
    //SocketPointer->exec_queue(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()));
    //QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
    LOG<<"activate code is "<<ParserResult.toString();
}
