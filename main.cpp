/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 13:09:20
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 14:06:33
 * @FilePath: \EOS\main.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "test/test.h"
#include "windows.h"
#include "DbgHelp.h"
#include "fileapi.h"

LONG CreateDumpFile(EXCEPTION_POINTERS *pException);
void initApp(QApplication& a);
#define use_python 1

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    initApp(a);
    MainWindow w;
    setWindowAlignCenter(&w);
    w.show();

    // mainwindow建立后再执行,否则异步消息会收不到,同步不影响
    SocketPointer->exec(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()),true);
    //SocketPointer->exec_queue(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()));
    //QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
    if (ParserResult.toBool()) LOG<<"[sync] socket is connect successful!";
    else LOG<<"[sync] socket is connect failed!";
    SocketPointer->exec(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()),true);
    //SocketPointer->exec_queue(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()));
    //QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
    LOG<<"[sync] activate code is "<<ParserResult.toString();

    // 相机也有信号需要等mainwindow建立
    Toupcam_GigeEnable(nullptr, nullptr);// 初始化对相机的支持
    if (!ToupCameraPointer->isOpen()){
        ToupCameraPointer->openCamera();
    };
    return QApplication::exec();
}

void initApp(QApplication& a)
{
    // 1. basic info
    LOG <<"UserScreen Width:" << UserScreenWidth << "UserScreen Height:" << UserScreenHeight;
    LOG<<"main thread is "<<CURRENT_THREAD;
    LOG<<"current path = "<<CURRENT_PATH;
    //    foreach(auto p, systemDrivers()) {
//        LOG<<p<<" total = "<<getDiskSpace(p,false)<<"MB free = "<<getDiskSpace(p)<<"MB "
//        << getDiskSpace(p) / getDiskSpace(p,false) * 100.0 <<"%";
//    }

    // 2. exception dump file
    LogInit;
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CreateDumpFile);

    // 3. qss/font
    QFile qss(":/style.qss");
    qss.open(QIODevice::ReadOnly);
    a.setStyleSheet(qss.readAll());
    a.setFont(QFont(DefaultFontFamily,DefaultFontSize));
    a.setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DesktopIcon));
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    qRegisterMetaType<QRequestMsg>("QRequestMsg");
    qRegisterMetaType<QRequestQueue>("QRequestQueue");

#ifdef use_python
    // 4. python
    // 这段Python进程的代码移动到initApp去写而不是在mainwindow.cpp的构造函数去写
    // 因为mainwindow.cpp构造完成之前可能已经触发了某些信号给服务端发送命令,例如独立于mainwindow构造的setting
    // 这样setting有关的信号发送的时候python进程尚未启动(mainwindow并未构造完)导致服务端没收到消息
    // 所以提前启动Python进程,这样mainwindow或者settng初始化发送某些信号时都能保证服务端已经正常
    // 现在方便测试自己去启动Python,以后变成自启动时这些代码也就不需要了,目前需要保留
    // 属于进程方式 测试方便,暂时需要保留,以后可以从这开始到代码结束都注释掉
    auto * process = new QProcess;
    // 22987 Lenovo
    process->start("C:\\Users\\22987\\AppData\\Local\\Programs\\Python\\Python310\\python.exe",
                   QStringList()<<"test_socket.py");
    process->waitForStarted();
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [process](){
        process->close();
        process->waitForFinished();
        LOG<<"python process is kill? "<<!process->isOpen();
    });
#endif
    // 5. quit thread
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [=]() {
        if (TimerBroadCastThreadPointer->isRunning()) {
            TimerBroadCastThreadPointer->stopThread();
            LOG<<"stop time broadcast thread";
        }
#ifdef use_imagetransformthread
        if (ImageTransformThreadPointer->isRunning()) {
            ImageTransformThreadPointer->stopThread();
            LOG<<"stop image transform thread";
        }
#endif
    });

    // 7. socket/tcp
    SocketInit; //注意: 先启动进程再启动连接
    LOG<<"is connect? "<<SocketPointer->isConnected()<<SocketPointer->socketState();
}

LONG CreateDumpFile(EXCEPTION_POINTERS *pException)
{
    // 创建Dump文件
    auto path = CURRENT_PATH +"\\"+QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss")+"_crash.dmp";
    LOG<<"dmp path = "<<path;
    auto path_s = path.toUtf8();
    HANDLE hDumpFile = CreateFile(path_s, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Dump信息
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    // 写入Dump文件内容
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    QMessageBox::critical(NULL,"严重错误","程序崩溃,异常退出");
    //CloseHandle(hDumpFile);
    return EXCEPTION_EXECUTE_HANDLER;
}
