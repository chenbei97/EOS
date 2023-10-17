/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 13:09:20
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 14:06:33
 * @FilePath: \EOS\main.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include <QApplication>
#include "slider.h"
#include "groupbox.h"
#include "mysqlreadwrite.h"
#include "pattern.h"

SQLType CurrentSqlType; // 必须定义

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    Slider w;
//    w.setSuffix("ms");
//    w.setPrefix("曝光时间: ");
//    w.setRange(0,15000);
//    w.show();
//    MysqlReadWritePointer->instance();
//    LOG<<CurrentSqliteType;
    Pattern w(16,24);
    w.toggleState(Pattern::TickState);
    //w.selectPoints(QPointVector()<<QPoint(1,1)<<QPoint(2,2));
    //w.clearPoints();
    //w.toggleState(Pattern::GroupState);
    w.resize(2000,1200);
    w.show();
    return QApplication::exec();
}
