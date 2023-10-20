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

int main(int argc, char *argv[]) {
    LogInit;
    QApplication a(argc, argv);
    initApp(a);


    //test_assemble_parse();
    //test_pattern();
    //test_tcp();

    MainWindow w;
    setWindowAlignCenter(&w);
    w.show();

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
}
