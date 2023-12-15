/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-15 08:48:42
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 09:10:28
 * @FilePath: \EOS\interface\include\mainwindow\pythoncall.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_PYTHONCALL_H
#define EOS_PYTHONCALL_H

#include "window.h"
#include "Python.h"

class INTERFACE_IMEXPORT PythonCall : public QObject
{
    Q_OBJECT
public:
    static PythonCall& instance();
    void start(const QString &path, const QString &file, const QString &func);
private:
    explicit PythonCall(QObject* parent= nullptr);
    ~PythonCall();
};

#define PythonCallPointer (&PythonCall::instance())
#endif //EOS_PYTHONCALL_H
