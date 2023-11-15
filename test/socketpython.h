/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 15:48:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 15:52:35
 * @FilePath: \EOS\test\socketpython.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SOCKETPYTHON_H
#define EOSI_SOCKETPYTHON_H

#include "public.h"

#ifdef use_python
#include "Python.h"
#endif

class SocketPython : public QThread
{
    Q_OBJECT
public:
    explicit SocketPython(QObject *parent = nullptr);
    ~SocketPython();
    void run() override;
};
#endif //EOSI_SOCKETPYTHON_H
