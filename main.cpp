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
//#define usetesttcp 0

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
//    QTextCodec *codec = QTextCodec::codecForName("utf-8");
//    QTextCodec::setCodecForLocale(codec);
    a.setFont(QFont(DefaultFontFamily,DefaultFontSize));
    //qRegisterMetaType<QList<int>>("QList<int>");
    a.setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DesktopIcon));
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    //QTimer::singleShot(0,qApp,SLOT(quit()));


    // 这段Python进程的代码移动到initApp去写而不是在mainwindow.cpp的构造函数去写
    // 因为mainwindow.cpp构造完成之前可能已经触发了某些信号给服务端发送命令,例如独立于mainwindow构造的setting
    // 这样setting有关的信号发送的时候python进程尚未启动(mainwindow并未构造完)导致服务端没收到消息
    // 所以提前启动Python进程,这样mainwindow或者settng初始化发送某些信号时都能保证服务端已经正常
#ifdef use_python
    auto process = new PythonProcess;
#ifndef  use_testSocket
    process->start("Eos_I/Eos_main.py");
    //StartPythonPointer->start("Eos_I","Eos_main","main");
#else
    process->start("../test/test_socket.py");
    //StartPythonPointer->start("../test","test_socket","test_server");
#endif
#else
    SocketInit;
    auto * process = new QProcess;
    process->start("C:\\Users\\22987\\AppData\\Local\\Programs\\Python\\Python310\\python.exe",//测试方便,暂时需要保留
                   QStringList()<<"../test/test_socket.py");
    process->waitForStarted();
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [process]() {
        process->close();
        process->waitForFinished();
        LOG<<"python process is kill? "<<!process->isOpen();
    });
    SocketPointer->exec(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()),true);
    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
    else LOG<<"socket is connect failed!";
    SocketPointer->exec(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()),true);
    LOG<<"activate code is "<<ParserResult.toString();
#endif
}
