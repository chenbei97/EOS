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


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Slider w;
    w.setSuffix("ms");
    w.setPrefix("曝光时间: ");
    w.setRange(0,15000);
    w.show();
    return QApplication::exec();
}
