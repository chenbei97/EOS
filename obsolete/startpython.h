/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:42:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 10:42:59
 * @FilePath: \EOS\interface\include\startpython.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_STARTPYTHON_H
#define EOSI_STARTPYTHON_H

#include "window.h"
#include "pythonthread.h"

class INTERFACE_IMEXPORT StartPython: public QObject
{
    Q_OBJECT
public:
    static StartPython& instance();
    void start(const QString &path, const QString &file, const QString &func);
    void quit();
private:
    explicit StartPython(QObject* parent= nullptr);
    ~StartPython();
    PythonThread * thread;
};

//static StartPython* StartPythonPointer =  &StartPython::instance();// 只能显示使用指针不会出现符号报错问题,不知道为啥
#define StartPythonPointer (&StartPython::instance())
#endif //EOSI_STARTPYTHON_H
