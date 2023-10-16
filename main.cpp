/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 13:09:20
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 14:06:33
 * @FilePath: \EOS\main.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include <QApplication>
#include "groupbox.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GroupBox w;
    w.show();
    return QApplication::exec();
}
