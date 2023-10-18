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

int main(int argc, char *argv[]) {
    LogInit;
    QApplication a(argc, argv);
    LOG << "Screen Width:" << UserScreenWidth << "Screen Height:" << UserScreenHeight;

    //test_assemble_parse();
    Pattern w(16,24);
    w.toggleState(Pattern::TickState);
    w.selectPoints(QPointVector()<<QPoint(1,1)<<QPoint(2,2));
    //w.clearPoints();
    w.resize(2000,1200);
//    SocketPanel w;
    w.show();
    return QApplication::exec();
}
