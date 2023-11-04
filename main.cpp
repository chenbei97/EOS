/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 13:09:20
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 14:06:33
 * @FilePath: \EOS\main.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "test/test.h"

SQLType CurrentSqlType; // 必须定义

void initApp(QApplication& a);
//#define usetesttcp 0

int main(int argc, char *argv[]) {
    LogInit;
    QApplication a(argc, argv);
    initApp(a);
    //StartPythonPointer->start("../test","test_socket","test_server");
    //test_camera();
    //test_assemble_parse();
    //test_async();
    //test_opencv();

    ToupCameraPointer->haveCamera();

#ifdef usetesttcp
    test_tcp();
#else
    MainWindow w;
    setWindowAlignCenter(&w);
    //TriangleMove w;
    w.show();
#endif
    return QApplication::exec();
}

void initApp(QApplication& a)
{
    LOG << "UserScreen Width:" << UserScreenWidth << "UserScreen Height:" << UserScreenHeight;
    LOG<<"main thread is "<<CURRENT_THREAD;
    LOG<<"current path = "<<CURRENT_PATH;
//#ifndef usetesttcp

//#endif
//    QTextCodec *codec = QTextCodec::codecForName("utf-8");
//    QTextCodec::setCodecForLocale(codec);
    a.setFont(QFont(DefaultFontFamily,DefaultFontSize));
    //qRegisterMetaType<QList<int>>("QList<int>");
    a.setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DesktopIcon));
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    //QTimer::singleShot(0,qApp,SLOT(quit()));
    QObject::connect(qApp,&QApplication::aboutToQuit, []{StartPythonPointer->quit();});
    //connect(&a,&QApplication::aboutToQuit,)
}
