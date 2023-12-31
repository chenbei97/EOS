/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-09 15:04:28
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-09 16:36:34
 * @FilePath: \EOS\interface\include\mainwindow\pythonprocess.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_PYTHONPROCESS_H
#define EOS_PYTHONPROCESS_H

#include "window.h"
#include "Python.h"

class INTERFACE_IMEXPORT PythonProcess: public QObject
{
    Q_OBJECT
public:
    explicit PythonProcess(QObject*parent= nullptr);
    void start(const QString & path);
private:
    QProcess * process = nullptr;
};

#endif //EOS_PYTHONPROCESS_H
